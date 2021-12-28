#ifndef BFD_ADAPTER_H
#define BFD_ADAPTER_H

// bfd.h needs to have PACKAGE and PACKAGE_VERSION defined
#define PACKAGE "cgreen-runner"
#define PACKAGE_VERSION "unknown"
#include <bfd.h>


#ifdef UNITTESTING
#define bfd_adapter_openr bfd_adapter_openr_unittesting
#define bfd_adapter_close bfd_adapter_close_unittesting
#define bfd_adapter_get_symtab_upper_bound bfd_adapter_get_symtab_upper_bound_unittesting
#define bfd_adapter_get_dynamic_symtab_upper_bound bfd_adapter_get_dynamic_symtab_upper_bound_unittesting
#define bfd_adapter_get_file_flags bfd_adapter_get_file_flags_unittesting
#define bfd_adapter_asymbol_bfd bfd_adapter_asymbol_bfd_unittesting
#define bfd_adapter_is_target_special_symbol bfd_adapter_is_target_special_symbol_unittesting
#define bfd_adapter_canonicalize_symtab bfd_adapter_canonicalize_symtab_unittesting
#define bfd_adapter_canonicalize_dynamic_symtab bfd_adapter_canonicalize_dynamic_symtab_unittesting
#define bfd_adapter_check_format bfd_adapter_check_format_unittesting
#define bfd_adapter_alloc bfd_adapter_alloc_unittesting
#endif

bfd *bfd_adapter_openr(const char *filename, const char *target);
bfd_boolean bfd_adapter_close(bfd *abfd);

long bfd_adapter_get_symtab_upper_bound(bfd *abfd);
long bfd_adapter_get_dynamic_symtab_upper_bound(bfd *abfd);
flagword bfd_adapter_get_file_flags(const bfd *abfd);

struct bfd *bfd_adapter_asymbol_bfd(const asymbol *sy);
bfd_boolean bfd_adapter_is_target_special_symbol(bfd *abfd, asymbol *symbols);
long bfd_adapter_canonicalize_symtab(bfd *abfd, asymbol **symbols);
long bfd_adapter_canonicalize_dynamic_symtab(bfd *abfd, asymbol **symbols);

bfd_boolean bfd_adapter_check_format(bfd *abfd, bfd_format format);
void *bfd_adapter_alloc(bfd *abfd, bfd_size_type wanted);

#endif
