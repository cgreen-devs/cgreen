#include <cgreen/breadcrumb.h>
#include <stdlib.h>

struct CgreenBreadcrumb_ {
    const char **trail;
    int depth;
    int space;
};

CgreenBreadcrumb *create_breadcrumb(void) {
    CgreenBreadcrumb *breadcrumb = malloc(sizeof(CgreenBreadcrumb));
    if (breadcrumb == NULL) {
        return NULL;
    }
	breadcrumb->trail = NULL;
	breadcrumb->depth = 0;
	breadcrumb->space = 0;
	return breadcrumb;
}

void destroy_breadcrumb(CgreenBreadcrumb *breadcrumb) {
	free(breadcrumb->trail);
	free(breadcrumb);
}

void push_breadcrumb(CgreenBreadcrumb *breadcrumb, const char *name) {
	breadcrumb->depth++;
	if (breadcrumb->depth > breadcrumb->space) {
        const char **tmp;
		breadcrumb->space++;
        tmp = realloc(breadcrumb->trail,
                sizeof(const char *) * breadcrumb->space);
        if (tmp == NULL) {
            breadcrumb->space--;
            breadcrumb->depth--;
            return;
        }
        breadcrumb->trail = tmp;
	}
	breadcrumb->trail[breadcrumb->depth - 1] = name;
}

void pop_breadcrumb(CgreenBreadcrumb *breadcrumb) {
	breadcrumb->depth--;
}

const char *get_current_from_breadcrumb(CgreenBreadcrumb *breadcrumb) {
    if (get_breadcrumb_depth(breadcrumb) == 0) {
        return NULL;
    }
	return breadcrumb->trail[breadcrumb->depth - 1];
}

int get_breadcrumb_depth(CgreenBreadcrumb *breadcrumb) {
    return breadcrumb->depth;
}

void walk_breadcrumb(CgreenBreadcrumb *breadcrumb, void (*walker)(const char *, void *), void *memo) {
    int i;
    for (i = 0; i < breadcrumb->depth; i++) {
        (*walker)(breadcrumb->trail[i], memo);
    }
}

/* vim: set ts=4 sw=4 et cindent: */
