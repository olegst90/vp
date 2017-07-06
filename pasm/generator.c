#include "generator.h"
#include "parser.h"
#include "pobj.h"
#include <stdio.h>
#include <strhelper.h>

#define PAGE_LEN 0x1000

int build_interbin(struct list_item *tokens, struct interbin *bin)
{
  uint32_t counter = 0;
  bin->bin = (uint8_t *)calloc(1, PAGE_LEN);
  if (!bin->bin) {
    fprintf(stderr, "Could not allocate buffer\n");
    return -1;
  }
  bin->binsize = PAGE_LEN;
  list_init(&bin->sym);
  list_init(&bin->symref);

  struct list_item *l = list_next(tokens);
  struct token *t = NULL;
  struct symbol *sym = NULL;
  struct symref *symref = NULL;
  while(l) {
    t = (struct token *)list_data(l);
    switch(t->type) {
      case NEWLINE: break;
      case LABEL:
         sym = (struct symbol *)calloc(1, sizeof(*sym));
         sym->type = SYMBOL_DEFALT;
         sym->offset = counter;
         strcpy(sym->name, t->v.s);
         list_add_data(&bin->sym, sym);
         break;
      case DIRECTIVE:
         if(streq(t->v.s, ".BYTE")) {
           l = list_next(l);
           t = (struct token *)list_data(l);
           if (t->type != LITERAL || t->subtype != INT) {
             fprintf(stderr, "Integer value expected on line %d\n", t->line);
             goto error;
           }
           if ((int)((uint8_t)t->v.i) != t->v.i) {
             fprintf(stderr, "Can't fit %d in 1 byte on line %d\n", t->v.i, t->line);
             goto error;
           }
           *(uint8_t *)(bin->bin + counter) = (uint8_t)(t->v.i);
           counter++;
         } else if(streq(t->v.s, ".WORD")) {
           l = list_next(l);
           t = (struct token *)list_data(l);
           if (t->type != LITERAL || t->subtype != INT) {
             fprintf(stderr, "Integer value expected on line %d\n", t->line);
             goto error;
           }
           if ((int)((uint16_t)t->v.i) != t->v.i) {
             fprintf(stderr, "Can't fit %d in 1 byte on line %d\n", t->v.i, t->line);
             goto error;
           }
           *(uint16_t *)(bin->bin + counter) = (uint16_t)(t->v.i);
           counter += 2;
         } else if (streq(t->v.s, ".STR")) {
           l = list_next(l);
           t = (struct token *)list_data(l);
           if (t->type != LITERAL || t->subtype != STR) {
             fprintf(stderr, "String value expected on line %d\n", t->line);
             goto error;
           }
           strcpy(bin->bin, t->v.s);
           counter += strlen(t->v.s);
         }
         break;
    }
    l = list_next(l);
  }
  bin->binsize = counter;
  return 0;
error:
  free(bin->bin);
  list_clear(&bin->sym);
  list_clear(&bin->symref);
  return -1;
}
