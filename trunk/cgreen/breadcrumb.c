#include "breadcrumb.h"
#include <stdlib.h>

Breadcrumb *create_breadcrumb() {
    Breadcrumb *breadcrumb = (Breadcrumb *)malloc(sizeof(Breadcrumb));
	breadcrumb->trail = NULL;
	breadcrumb->depth = 0;
	breadcrumb->space = 0;
	return breadcrumb;
}

void destroy_breadcrumb(Breadcrumb *breadcrumb) {
	free(breadcrumb->trail);
	free(breadcrumb);
}

void push_breadcrumb(Breadcrumb *breadcrumb, const char *name) {
	breadcrumb->depth++;
	if (breadcrumb->depth > breadcrumb->space) {
		breadcrumb->space++;
		breadcrumb->trail = (const char **)realloc(
				breadcrumb->trail,
				sizeof(const char *) * breadcrumb->space);
	}
	breadcrumb->trail[breadcrumb->depth - 1] = name;
}

void pop_breadcrumb(Breadcrumb *breadcrumb) {
	breadcrumb->depth--;
}

const char *get_current_from_breadcrumb(Breadcrumb *breadcrumb) {
    if (get_breadcrumb_depth(breadcrumb) == 0) {
        return NULL;
    }
	return breadcrumb->trail[breadcrumb->depth - 1];
}

int get_breadcrumb_depth(Breadcrumb *breadcrumb) {
    return breadcrumb->depth;
}

void walk_breadcrumb(Breadcrumb *breadcrumb, void (*walker)(const char *, void *), void *memo) {
    int i;
    for (i = 0; i < breadcrumb->depth; i++) {
        (*walker)(breadcrumb->trail[i], memo);
    }
}
