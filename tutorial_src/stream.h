char *read_paragraph(int (*read)(void *), void *stream);
void by_paragraph(int (*read)(void *), void *in, void (*write)(void *, char *), void *out);
