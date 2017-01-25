#include <stdlib.h>
#include <string.h>
#include "utils.h"


char *string_dup(const char *string) {
    char *dup = (char *)malloc(strlen(string)+1);
    if (dup)
        strcpy(dup, string);
    return dup;
}
