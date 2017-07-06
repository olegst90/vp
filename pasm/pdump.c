#include <stdio.h>
#include <stdlib.h>
#include "pobj.h"


void usage() {
  printf("Usage: pdump <pobj>\n");
}

const char *sym_type_to_str(int type) {
  switch(type) {
    case SYMBOL_DEFAULT: return "DEFAULT";
    case SYMBOL_EXPORTED: return "EXPORTED";
    case SYMBOL_NONE: return "NONE";
    default: return "<err>";
  };
}

const char *ref_type_to_str(int type) {
  switch(type) {
    case SYMREF_ABS: return "ABS";
    case SYMREF_REL: return "REL";
    default: return "<err>";
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    usage();
    return -1;
  }
  FILE *f = fopen(argv[1], "rb");
  if (!f) {
    fprintf(stderr, "Could not open %s\n", argv[1]);
    return -1;
  }

  fseek(f, 0, SEEK_END);
  long int fsize = ftell(f);
  fseek(f, 0, SEEK_SET);

  uint8_t *pobj = (uint8_t *)malloc(fsize);
  if (!pobj) {
    fclose(f);
    fprintf(stderr, "Could not allocate buffer\n");
    return -1;
  }
  if (fread(pobj, 1, fsize, f) != fsize ) {
    fclose(f);
    fprintf(stderr, "Could not read all file\n");
    return -1;
  }
  fclose(f);

  struct pobj_header *header = (struct pobj_header *)pobj;

  if ( header->sym_section + header->sym_section_len > fsize ||
       header->ref_section + header->ref_section_len > fsize ||
       header->bin_section + header->bin_section_len > fsize ||
       header->sym_section_len % sizeof(struct symbol) ||
       header->ref_section_len % sizeof(struct symref)
     )

  {
    fprintf(stderr, "Corrupted header\n");
    free(pobj);
    return -1;
  }

  printf("HEADER\n\n");
  dump_header(header);
  printf("\nSYMBOLS\n\n");

  struct symbol *symbols = (struct symbol *)(pobj + header->sym_section);
  int num = header->sym_section_len / sizeof(struct symbol);
  int i;
  for (i = 0; i < num; i++) {
    printf("%.32s\t%.10s\t%#08X\n", symbols[i].name, sym_type_to_str(symbols[i].type), symbols[i].offset);
  }

  printf("\nSYMREFS\n\n");

  struct symref *symrefs = (struct symref *)(pobj + header->ref_section);
  num = header->ref_section_len / sizeof(struct symref);
  for (i = 0; i < num; i++) {
    printf("%.32s\t%.10s\t%#08X\n", symrefs[i].name, ref_type_to_str(symrefs[i].type), symrefs[i].offset);
  }

  return 0;
}
