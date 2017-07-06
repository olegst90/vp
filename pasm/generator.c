#include "generator.h"
#include "parser.h"
#include "pobj.h"
#include <stdio.h>
#include <strhelper.h>
#include <cpu_default_defines.h>

#define PAGE_LEN 0x1000

static struct symbol *find_symbol(struct list_item *syms, const char *name)
{
   struct list_item *p = list_next(syms);
   struct symbol *s;
   while (p) {
     s = (struct symbol *)list_data(p);
     if (streq(name, s->name)) {
       return s;
     }
     p = list_next(p);
   }
   return NULL;
}

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
      case SYMBOL:
         sym = find_symbol(&bin->sym, t->v.s);
         if (sym) {
           if (sym->offset == SYMBOL_OFFSET_NONE) {
             sym->offset = counter;
           } else {
             fprintf(stderr, "%s already defined\n", sym->name);
             goto error;
           }
         } else {
           sym = (struct symbol *)calloc(1, sizeof(*sym));
           sym->type = SYMBOL_DEFAULT;
           sym->offset = counter;
           strcpy(sym->name, t->v.s);
           list_add_data(&bin->sym, sym);
         }
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
         } else if (streq(t->v.s, ".GLOBAL")) {
           l = list_next(l);
           t = (struct token *)list_data(l);
           if (t->type != SYMREF) {
             fprintf(stderr, ".GLOBAL requires symref (line %d)\n", t->line);
             goto error;
           }
           sym = find_symbol(&bin->sym, t->v.s);
           if (sym) {
             sym->type = SYMBOL_EXPORTED;
           } else {
             sym = (struct symbol *)calloc(1, sizeof(*sym));
             sym->type = SYMBOL_DEFAULT;
             sym->offset = SYMBOL_OFFSET_NONE;
             strcpy(sym->name, t->v.s);
             list_add_data(&bin->sym, sym);
           }
         }
         break;
      case INSTRUCTION:
         if(streq(t->v.s, "JMPF")) {
           l = list_next(l);
           t = (struct token *)list_data(l);
           uint16_t addr = 0;
           printf("JMPF arg type %d\n", t->type);
           if (t->type == LITERAL && t->subtype == INT) {
              addr = (uint16_t)t->v.i;
              if ((int)addr != t->v.i) {
                fprintf(stderr, "can't fit addr in 2 bytes (line %d)\n", t->line);
                goto error;
              }
           } else if (t->type == SYMREF) {
             symref = (struct symref *)calloc(1, sizeof(*symref));
             strcpy(symref->name, t->v.s);
             symref->type = SYMREF_ABS;
             symref->offset = counter + 1;
             list_add_data(&bin->symref, symref);
           }
           *(uint8_t *)(bin->bin + counter) = (uint8_t)CPU_DEFAULT_JMPF;
           *(uint16_t *)(bin->bin + counter + 1) = (uint16_t)addr;
           counter += 3;
         }
         break;
    }
    l = list_next(l);
  }
  bin->binsize = counter;
  //check symbols
  l = list_next(&bin->sym);
  while (l) {
    sym = (struct symbol *)list_data(l);
    if (sym->offset == SYMBOL_OFFSET_NONE) {
      fprintf(stderr, "Symbol %s referenced but not defined\n", sym->name);
      goto error;
    }
    l = list_next(l);
  }

  //resolve internal symrefs
  l = list_next(&bin->symref);
  struct list_item *l2;
  while (l) {
    symref = (struct symref *)list_data(l);
    sym = find_symbol(&bin->sym, symref->name);
    if (sym) {
      l2 = l;
      l = list_next(l);
      list_remove(&bin->symref, l2);
    } else {
       l = list_next(l);
    }
  }
  return 0;
error:
  free(bin->bin);
  bin->bin = NULL;
  list_clear(&bin->sym);
  list_clear(&bin->symref);
  return -1;
}
