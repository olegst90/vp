#include "pobj.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void dump_header(struct pobj_header *header){
  printf("bin section: %#x:%u\n", header->bin_section, header->bin_section_len);
  printf("sym section: %#x:%u\n", header->sym_section, header->sym_section_len);
  printf("ref section: %#x:%u\n", header->ref_section, header->ref_section_len);
}

void dump_symbol(struct symbol *sym) {
  printf("name: %s\n", sym->name);
}

int build_pobj(const char *path, uint8_t *bin, uint32_t bin_len, struct list_item *sym, struct list_item *symref)
{
  int res = -1;
  FILE *f = fopen(path, "w+");
  if (!f) {
    fprintf(stderr, "Can't open %s\n", path);
    return -1;
  }
  struct pobj_header header;
  memset(&header, sizeof(header), 0);

  uint32_t symbin_len = list_len(sym)*sizeof(struct symbol);
  uint8_t *symbin = (uint8_t *)calloc(1,symbin_len);
  if (!symbin) {
    fprintf(stderr, "Can't allocate buffer for sym table\n");
    goto cleanup;
  }
  uint8_t *pbin = symbin;;
  struct list_item *p = list_next(sym);
  struct symbol *s;
  while(p) {
    s = (struct symbol *)list_data(p);
//    dump_symbol(s);
    memcpy(pbin, s, sizeof(*s));
    pbin += sizeof(*s);
    p = list_next(p);
  }
  uint32_t symrefbin_len = list_len(symref)*sizeof(struct symref);
  uint8_t *symrefbin = (uint8_t *)calloc(1,symrefbin_len);
  if (!symrefbin) {
    fprintf(stderr, "Can't allocate buffer for symref table\n");
    goto cleanup;
  }

  p = list_next(symref);
  pbin = symrefbin;
  struct symref *sr;
  while(p) {
    sr = (struct symref *)list_data(p);
    printf("copy symref %s\n", sr->name);
    memcpy(pbin, sr, sizeof(*sr));
    pbin += sizeof(*sr);
    p = list_next(p);
  }

  header.bin_section = sizeof(header);
  header.bin_section_len = bin_len;
  header.sym_section = header.bin_section + bin_len;
  header.sym_section_len = symbin_len;
  header.ref_section = header.sym_section + symbin_len;
  header.ref_section_len = symrefbin_len;

  size_t n = fwrite(&header, 1, sizeof(header), f) +
             fwrite(bin, 1, bin_len, f) +
             fwrite(symbin, 1, symbin_len, f) +
             fwrite(symrefbin, 1, symrefbin_len, f);

  if (n != (sizeof(header) + bin_len + symbin_len + symrefbin_len)) {
    fprintf(stderr, "Could not write all sections\n");
    goto cleanup;
  }
  dump_header(&header);
  res = 0;
cleanup:
  fclose(f);
  free(symbin);
  free(symrefbin);
  if(res != 0) remove(path);
  return res;

}
