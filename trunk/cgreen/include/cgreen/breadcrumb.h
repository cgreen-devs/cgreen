#ifndef BREADCRUMB_HEADER
#define BREADCRUMB_HEADER

typedef struct Breadcrumb_ {
    const char **trail;
    int depth;
    int space;
} Breadcrumb;

Breadcrumb *create_breadcrumb();
void destroy_breadcrumb(Breadcrumb *breadcrumb);
void push_breadcrumb(Breadcrumb *breadcrumb, const char *name);
void pop_breadcrumb(Breadcrumb *breadcrumb);
const char *get_current_from_breadcrumb(Breadcrumb *breadcrumb);
int get_breadcrumb_depth(Breadcrumb *breadcrumb);
void walk_breadcrumb(Breadcrumb *breadcrumb, void (*walker)(const char *, void *), void *memo);

#endif
