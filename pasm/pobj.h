#ifndef POBJ_H
#define POBJ_H

#include <stdint.h>
#include <list.h>

#define MAX_SYM_LEN 64

enum symbol_type {
  SYMBOL_DEFAULT, SYMBOL_EXPORTED, SYMBOL_NONE
};

#define SYMBOL_OFFSET_NONE ((uint32_t)-1U)

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

void dump_header(struct pobj_header *header);
void dump_symbol(struct symbol *sym);

int build_pobj(const char *path, uint8_t *bin, uint32_t bin_len, struct list_item *sym, struct list_item *symref);

#endif // POBJ_H
