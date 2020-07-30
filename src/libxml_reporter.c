#include <cgreen/breadcrumb.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>
#include <assert.h>

#include <libxml/xmlwriter.h>
#include <libxml/encoding.h>
#include <libxml/xmlIO.h>
#include <libxml/tree.h>

#include "libxml_reporter_internal.h"

#ifdef __ANDROID__
#include "cgreen/internal/android_headers/androidcompat.h"
#endif // #ifdef __ANDROID__

typedef struct {
    XmlPrinter *printer;
    int segment_count;
} XmlMemo;

#define XMLSTRING(x) (BAD_CAST x)

static void xml_reporter_start_suite(TestReporter *reporter, const char *name,
                                     int count);
static void xml_reporter_start_test(TestReporter *reporter, const char *name);
static void xml_reporter_finish_test(TestReporter *reporter, const char *filename,
                                     int line, const char *message);
static void xml_reporter_finish_suite(TestReporter *reporter, const char *filename,
                                      int line);
static void xml_show_skip(TestReporter *reporter, const char *file, int line);
static void xml_show_fail(TestReporter *reporter, const char *file, int line,
                          const char *message, va_list arguments);
static void xml_show_incomplete(TestReporter *reporter, const char *filename,
                                int line, const char *message, va_list arguments);

static const char *file_prefix;

static int default_printer(xmlDocPtr doc);

void set_libxml_reporter_printer(TestReporter *reporter, XmlPrinter *printer)
{
    XmlMemo *memo = (XmlMemo*)reporter->memo;
    memo->printer = printer;
}

TestReporter *create_libxml_reporter(const char *prefix) {
    TestReporter *reporter;
    XmlMemo *memo;

    reporter = create_reporter();
    if (reporter == NULL) {
        return NULL;
    }

    memo = (XmlMemo *) malloc(sizeof(XmlMemo));
    if (memo == NULL) {
        destroy_reporter(reporter);
        return NULL;
    }
    reporter->memo = memo;
    memo->printer = &default_printer;

    file_prefix = prefix;
    reporter->start_suite = &xml_reporter_start_suite;
    reporter->start_test = &xml_reporter_start_test;
    reporter->show_fail = &xml_show_fail;
    reporter->show_skip = &xml_show_skip;
    reporter->show_incomplete = &xml_show_incomplete;
    reporter->finish_test = &xml_reporter_finish_test;
    reporter->finish_suite = &xml_reporter_finish_suite;
    return reporter;
}

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif
static char suite_path[PATH_MAX];

static void print_path_separator_if_needed(int *more_segments) {
    if (*more_segments > 0) {
        strcat(suite_path, "/");
        (*more_segments)--;
    }
}

static void print_path_segment_walker(const char *segment, void *void_memo) {
    XmlMemo *memo = (XmlMemo *)void_memo;

    strncat(suite_path, segment, sizeof(suite_path)-strlen(suite_path)-1);
    print_path_separator_if_needed(&memo->segment_count);
}

static void strcat_path_segment(const char *segment, void *more_segments) {
    (void)more_segments;
    if (suite_path[0] != '\0') strcat(suite_path, "-");
    strncat(suite_path, segment, sizeof(suite_path)-strlen(suite_path)-1);
}

static void add_suite_name(const char *suite_name) {
    if (suite_path[0] != '\0')
        strcat(suite_path, "-");
    strncat(suite_path, suite_name, sizeof(suite_path)-strlen(suite_path)-1);
}

#define NESTED_SUITE_MAX 100
struct xml_suite_context {
    xmlDocPtr doc;
    xmlNodePtr suite, curTest;
    FILE* outFile;
    uint32_t suite_duration;
};
static struct xml_suite_context context_stack[NESTED_SUITE_MAX];
static int context_stack_p = 0;

/*
 * The table below is taken from the "Non-restricted characters" section of
 * https://en.wikipedia.org/wiki/Valid_characters_in_XML
 */
static bool isNonRestrictedXMLChar(unsigned int c) {
    return (c == 0x09u || c == 0x0Au || c == 0x0Du ||
            (c >= 0x20u && c <= 0x7Eu) || c == 0x85u ||
            (c >= 0xA0u && c <= 0xD7FFu) ||
            (c >= 0xE000u && c <= 0xFDCFu) ||
            (c >= 0xFDF0u && c <= 0xFFFDu) ||
            (c >= 0x10000u && c <= 0x1FFFDu) ||
            (c >= 0x20000u && c <= 0x2FFFDu) ||
            (c >= 0x30000u && c <= 0x3FFFDu) ||
            (c >= 0x40000u && c <= 0x4FFFDu) ||
            (c >= 0x50000u && c <= 0x5FFFDu) ||
            (c >= 0x60000u && c <= 0x6FFFDu) ||
            (c >= 0x70000u && c <= 0x7FFFDu) ||
            (c >= 0x80000u && c <= 0x8FFFDu) ||
            (c >= 0x90000u && c <= 0x9FFFDu) ||
            (c >= 0xA0000u && c <= 0xAFFFDu) ||
            (c >= 0xB0000u && c <= 0xBFFFDu) ||
            (c >= 0xC0000u && c <= 0xCFFFDu) ||
            (c >= 0xD0000u && c <= 0xDFFFDu) ||
            (c >= 0xE0000u && c <= 0xEFFFDu) ||
            (c >= 0xF0000u && c <= 0xFFFFDu) ||
            (c >= 0x100000u && c <= 0x10FFFD));
}

static bool isOverlongUTF8(unsigned int ucs, int len)
{
    return ((ucs <= 0x7f && len > 1) ||
            (ucs <= 0x7ff && len > 2) ||
            (ucs <= 0xffff && len > 3) ||
            (ucs <= 0x10ffff && len > 4));
}

/*
 * Return a copy of the argument prepared to be used as an attribute value.
 * The produced result is not necessary convertable back to the original set
 * of bytes.
 */
static xmlChar* xmlEscapePropValue(const char *str) {
    size_t len = strlen(str);
    /*
     * The worst-case length of the output is 4 times the input length (if
     * every input byte has to be escaped) plus 1 for the terminating NUL
     * character
     */
    size_t retLen = len * 4 + 1;
    xmlChar* ret = xmlMalloc(retLen), *retPos = ret, *retEnd = ret + retLen;
    if (!ret) {
        fprintf(stderr, "memory allocation failure in %s\n", __func__);
        exit(EXIT_FAILURE);
    }

    const unsigned char *it = (const unsigned char*)str;
    const unsigned char *itEnd = it + len;
    while (*it) {
        int utfLen = itEnd - it;
        int ucs = xmlGetUTF8Char(it, &utfLen);
        if (ucs != -1) {
            if (!isOverlongUTF8(ucs, utfLen)) {
                if (isNonRestrictedXMLChar(ucs)) {
                    /* Valid UTF8 sequence of an allowed character */
                    while (utfLen--)
                        *retPos++ = *it++;
                } else {
                    xmlStrPrintf(retPos, retEnd - retPos, "&x%x;", ucs);
                }
            } else {
                /* Disallowed character or overlong UTF8, escape entire sequence */
                for(int i = 0;i < utfLen;++i) {
                    xmlStrPrintf(retPos, retEnd-retPos, "\\x%.2" PRIx8, *it);
                    retPos += 4;
                    ++it;
                }
            }
        } else {
            /* Invalid UTF8, escape one byte then try to parse rest of input as UTF8 again */
            xmlStrPrintf(retPos, retEnd-retPos, "\\x%.2" PRIx8, *it);
            retPos += 4;
            ++it;
        }
    }

    *retPos = '\0';
    return ret;
}

static void xml_reporter_start_suite(TestReporter *reporter, const char *suitename, int count) {
    char filename[PATH_MAX];
    int segment_decrementer = reporter->breadcrumb->depth;
    XmlMemo *memo = (XmlMemo *)reporter->memo;
    if (context_stack_p >= NESTED_SUITE_MAX)
        abort();
    struct xml_suite_context *ctx = &context_stack[context_stack_p++];

    FILE *out;

    (void)count;                /* UNUSED */

    reporter->passes = 0;
    reporter->failures = 0;
    reporter->skips = 0;
    reporter->exceptions = 0;

    suite_path[0] = '\0';
    walk_breadcrumb(reporter->breadcrumb, strcat_path_segment, &segment_decrementer);
    add_suite_name(suitename);

    if (snprintf(filename, sizeof(filename), "%s-%s.xml", file_prefix, suite_path) >= (int)sizeof(filename)) {
        fprintf(stderr, "filename truncated; exceeds PATH_MAX (= %d)\n", PATH_MAX);
        exit(EXIT_FAILURE);
    }
    if (memo->printer == default_printer) {
        // If we're really printing to files, then open one...
        out = fopen(filename, "w");
        if (!out) {
            fprintf(stderr, "could not open %s: %s\r\n", filename, strerror(errno));
            exit(EXIT_FAILURE);
        }
        ctx->outFile = out;
    }

    ctx->suite_duration = 0;
    ctx->doc = xmlNewDoc(XMLSTRING("1.0"));
    ctx->suite = xmlNewNode(NULL, XMLSTRING("testsuite"));
    xmlChar *xml_suite_path = xmlEscapePropValue(suite_path);
    xmlNewProp(ctx->suite, XMLSTRING("name"), xml_suite_path);
    xmlFree(xml_suite_path);
    xmlDocSetRootElement(ctx->doc, ctx->suite);

    reporter_start_suite(reporter, suitename, 0);
}


/* Accumulate output from the actual test (the "<testcase>" nodes) in
   a file since the tests usually are run in a child processes, so
   there is no simple way to save output from it and then use it in
   the parent (start_test() and finish_test() are run from parent) */

static FILE *child_output_tmpfile;
static xmlTextWriterPtr child_output_writer;

static void xml_reporter_start_test(TestReporter *reporter, const char *testname) {
    XmlMemo *memo = (XmlMemo *)reporter->memo;
    struct xml_suite_context *ctx = &context_stack[context_stack_p-1];

    ctx->curTest = xmlNewChild(ctx->suite, NULL, XMLSTRING("testcase"), NULL);
    xmlChar *xml_testname = xmlEscapePropValue(testname);
    xmlNewProp(ctx->curTest, XMLSTRING("name"), xml_testname);
    xmlFree(xml_testname);

    memo->segment_count = reporter->breadcrumb->depth - 1;
    suite_path[0] = '\0';
    walk_breadcrumb(reporter->breadcrumb, print_path_segment_walker, memo);
    xmlChar* xml_suite_path = xmlEscapePropValue(suite_path);
    xmlNewProp(ctx->curTest, XMLSTRING("classname"), xml_suite_path);
    xmlFree(xml_suite_path);

    reporter_start_test(reporter, testname);

    child_output_tmpfile = tmpfile();
    xmlOutputBufferPtr tmpfileBuf
        = xmlOutputBufferCreateFile(child_output_tmpfile,
                                    xmlGetCharEncodingHandler(XML_CHAR_ENCODING_UTF8));
    child_output_writer = xmlNewTextWriter(tmpfileBuf);
}


static void xml_show_skip(TestReporter *reporter, const char *file, int line) {
    (void)file;
    (void)line;
    (void)reporter;

    xmlTextWriterStartElement(child_output_writer, XMLSTRING("skipped"));
    xmlTextWriterEndElement(child_output_writer); // </skipped>
    xmlTextWriterFlush(child_output_writer);
}

static xmlChar* xml_secure_vprint(const char *format, va_list ap)
{
    char buf[100];
    char *msg = buf;
    const size_t len = sizeof(buf);

    va_list ap_store;
    va_copy(ap_store, ap);
    int res = vsnprintf(msg, len, format, ap_store);
    va_end(ap_store);
    if (res < 0) {
        fprintf(stderr, "vsnprintf failed in xml_secure_vprint: %s\n", strerror(res));
        exit(EXIT_FAILURE);
    }

    if ((unsigned)res >= len) {
        size_t msglen = res+1;
        msg = malloc(msglen);
        if (!msg) {
            fprintf(stderr, "memory allocation failure in xml_secure_vprint\n");
            exit(EXIT_FAILURE);
        }
        res = vsnprintf(msg, msglen, format, ap);
        if (res < 0) {
            free(msg);
            fprintf(stderr, "vsnprintf failed in xml_secure_vprint: %s\n", strerror(res));
            exit(EXIT_FAILURE);
        }
        if ((unsigned)res >= msglen) {
            // What? Did format get longer while allocating msg?
            free(msg);
            fprintf(stderr, "vsnprintf failed in xml_secure_vprint: nondeterministic message length\n");
            exit(EXIT_FAILURE);
        }
    }

    xmlChar *xml_msg = xmlEscapePropValue(msg);
    if (msg != buf) free(msg);
    return xml_msg;
}

static void xml_show_fail(TestReporter *reporter, const char *file, int line,
                          const char *message, va_list arguments) {
    (void)reporter;

    xmlTextWriterStartElement(child_output_writer, XMLSTRING("failure"));

    xmlChar *xml_str = xml_secure_vprint(message, arguments);
    xmlTextWriterWriteAttribute(child_output_writer, XMLSTRING("message"), xml_str);
    xmlFree(xml_str);

    xmlTextWriterStartElement(child_output_writer, XMLSTRING("location"));
    xmlChar *xml_file = xmlEscapePropValue(file);
    xmlTextWriterWriteAttribute(child_output_writer, XMLSTRING("file"), xml_file);
    xmlFree(xml_file);
    xmlTextWriterWriteFormatAttribute(child_output_writer, XMLSTRING("line"), "%d", line);
    xmlTextWriterEndElement(child_output_writer); // </location>
    xmlTextWriterEndElement(child_output_writer); // </failure>
    xmlTextWriterFlush(child_output_writer);
}

static xmlAttrPtr xmlFormatProp(xmlNodePtr node, const xmlChar* name, const char *format, ...)
{
    va_list vargs;
    va_start(vargs, format);
    xmlChar *xml_str = xml_secure_vprint(format, vargs);
    va_end(vargs);

    xmlAttrPtr attr = xmlNewProp(node, name, xml_str);
    xmlFree(xml_str);

    return attr;
}

static void xml_show_incomplete(TestReporter *reporter, const char *filename,
                                int line, const char *message, va_list arguments) {
    (void)reporter;

    struct xml_suite_context *ctx = &context_stack[context_stack_p-1];

    xmlNodePtr errNode = xmlNewChild(ctx->curTest, NULL, XMLSTRING("error"), NULL);
    xmlNewProp(errNode, XMLSTRING("type"), XMLSTRING("Fatal"));
    if (message) {
        xmlChar *xml_msg = xml_secure_vprint(message, arguments);
        xmlNewProp(errNode, XMLSTRING("message"), xml_msg);
        xmlFree(xml_msg);
    } else {
        xmlNewProp(errNode, XMLSTRING("message"),
                   XMLSTRING("Test terminated unexpectedly, likely from a non-standard exception or Posix signal"));
    }
    xmlNodePtr locNode = xmlNewChild(errNode, NULL, XMLSTRING("location"), NULL);
    xmlChar *xml_filename = xmlEscapePropValue(filename);
    xmlNewProp(locNode, XMLSTRING("file"), xml_filename);
    xmlFree(xml_filename);
    xmlFormatProp(locNode, XMLSTRING("line"), "%d", line);
}

static void insert_child_results(struct xml_suite_context *ctx) {
    char childData[4096];
    fseek(child_output_tmpfile, 0, SEEK_SET);

    size_t pos = 0, ret;
    while (!feof(child_output_tmpfile)) {
        ret = fread(childData+pos, 1, sizeof(childData)-pos, child_output_tmpfile);
        if (ferror(child_output_tmpfile)) {
            abort();
        }
        pos += ret;
    }

    fclose(child_output_tmpfile);
    if (pos > 0) {
        childData[pos] = '\0';

        xmlNodePtr childLst;
        if (xmlParseBalancedChunkMemoryRecover(ctx->doc, NULL, NULL, 0, XMLSTRING(childData), &childLst, 1) != 0) {
            xmlNodePtr errNode = xmlNewChild(ctx->curTest, NULL, XMLSTRING("error"), NULL);
            xmlNewProp(errNode, XMLSTRING("type"), XMLSTRING("Fatal"));
            xmlNewProp(errNode, XMLSTRING("message"),
                       XMLSTRING("Test result XML truncated or malformed, "
                                 "likely from abnormal process termination"));
        }

        xmlAddChildList(ctx->curTest, childLst);
    }
}

static void xml_reporter_finish_test(TestReporter *reporter, const char *filename,
                                     int line, const char *message) {
    struct xml_suite_context *ctx = &context_stack[context_stack_p-1];

    reporter_finish_test(reporter, filename, line, message);
    xmlFreeTextWriter(child_output_writer);

    xmlFormatProp(ctx->curTest, XMLSTRING("time"),
                  "%.5f", (double)reporter->duration/(double)1000);

    ctx->suite_duration += reporter->duration;
    insert_child_results(ctx);
}

static void deleteEmpty(xmlNodePtr elem)
{
    while (elem != NULL) {
        xmlNodePtr next = elem->next;
        if (xmlIsBlankNode(elem)) {
            xmlUnlinkNode(elem);
            xmlFreeNode(elem);
        } else if (elem->children) {
            deleteEmpty(elem->children);
        }

        elem = next;
    }
}

static int default_printer(xmlDocPtr doc)
{
    struct xml_suite_context *ctx = &context_stack[context_stack_p-1];
    xmlOutputBufferPtr outBuf
        = xmlOutputBufferCreateFile(ctx->outFile,
                                    xmlGetCharEncodingHandler(XML_CHAR_ENCODING_UTF8));
    xmlSaveFormatFileTo(outBuf, doc, "UTF-8", 1);

    return 0;
}

static void xml_reporter_finish_suite(TestReporter *reporter, const char *filename, int line) {
    XmlMemo *memo = (XmlMemo *)reporter->memo;
    struct xml_suite_context *ctx = &context_stack[context_stack_p-1];

    reporter_finish_suite(reporter, filename, line);

    reporter->total_passes += reporter->passes;
    reporter->total_failures += reporter->failures;
    reporter->total_skips += reporter->skips;
    reporter->total_exceptions += reporter->exceptions;

    xmlFormatProp(ctx->suite, XMLSTRING("failures"), "%d", reporter->failures);
    xmlFormatProp(ctx->suite, XMLSTRING("errors"), "%d", reporter->exceptions);
    xmlFormatProp(ctx->suite, XMLSTRING("skipped"), "%d", reporter->skips);
    xmlFormatProp(ctx->suite, XMLSTRING("time"), "%.5f", (double)ctx->suite_duration/(double)1000);

    deleteEmpty(ctx->suite);
    memo->printer(ctx->doc);
    xmlFreeDoc(ctx->doc);
    if (context_stack_p > 1) {
        context_stack[context_stack_p-2].suite_duration += ctx->suite_duration;
    }
    --context_stack_p;
}
