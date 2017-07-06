#ifndef POBJ_H
#define POBJ_H

#include <stdint.h>

#define MAX_SYM_LEN 64

enum symbol_type {
  SYMBOL_DEFALT, SYMBOL_NONE
};

struct symbol {
  char name[MAX_SYM_LEN];
  uint32_t type;
  uint32_t offset;
};

enum symref_type{
  SYMREF_ABS, SYMREF_REL, SYMREF_NONE
};

struct symref {
  char name[MAX_SYM_LEN];
  uint32_t offset;
  uint32_t type;
};

#define MAX_BIN_LEN 0x1000

struct pobj_header {
  uint32_t bin_section;
  uint32_t bin_section_len;
  uint32_t sym_section;
  uint32_t sym_section_len;
  uint32_t ref_section;
  uint32_t ref_section_len;
};

#endif // POBJ_H
