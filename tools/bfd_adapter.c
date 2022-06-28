#include "bfd_adapter.h"

#include <stdio.h>
/* Adapter to libbfd */
bfd *bfd_adapter_openr(const char *filename, const char *target) {

    return bfd_openr(filename, target);
}

bfd_boolean bfd_adapter_close(bfd *abfd){
    return bfd_close(abfd);
}

long bfd_adapter_get_dynamic_symtab_upper_bound(bfd *abfd) {
    return bfd_get_dynamic_symtab_upper_bound(abfd);
}

flagword bfd_adapter_get_file_flags(const bfd *abfd){
    return bfd_get_file_flags(abfd);
}

struct bfd *bfd_adapter_asymbol_bfd(const asymbol *sy) {
    return bfd_asymbol_bfd(sy);
}

bfd_boolean bfd_adapter_is_target_special_symbol(bfd *abfd, asymbol *symbols) {
    return bfd_is_target_special_symbol(abfd, symbols);
}

long bfd_adapter_canonicalize_dynamic_symtab(bfd *abfd, asymbol **symbols) {
    return bfd_canonicalize_dynamic_symtab(abfd, symbols);
}

bfd_boolean bfd_adapter_check_format(bfd *abfd, bfd_format format) {
    return bfd_check_format(abfd, format);
}

void *bfd_adapter_alloc(bfd *abfd, bfd_size_type wanted) {
    return bfd_alloc(abfd, wanted);
}
