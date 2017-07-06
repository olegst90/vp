#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cpu_default_defines.h>
#include <list.h>
#include <strhelper.h>
#include "parser.h"
#include "pobj.h"
#include "generator.h"

void usage()
{
  printf("pasm <input> <output>\n");
}

int main(int argc, char **argv)
{
  if (argc != 3) {
    usage();
    return -1;
  }
  int res = -1;
  const char *inpath = argv[1], *outpath = argv[2];

  /*struct interbin out;
  list_init(&out.sym);
  list_init(&out.symref);
  out.bin = (uint8_t *)malloc(MAX_BIN_SZ);
  if (!out.bin) {
    fprintf(stderr, "Could not allocate outbuffer\n");
    goto cleanup;
  }
  out.binsize = 0;
  */

  struct list_item tokens;
  list_init(&tokens);

  if (0 != parse_file(inpath, &tokens)) {
    fprintf(stderr, "Parsing of %s failed\n", inpath);
    goto cleanup;
  }

  struct list_item *p = &tokens;
  struct token *t;
  while (p = list_next(p)) {
    t = (struct token *)list_data(p);
    printf("%s",token_to_str(t));
    if (t->type == NEWLINE) printf("\n");
  }

  struct interbin ibin;
  if (0 != build_interbin(&tokens, &ibin)) {
    fprintf(stderr,"Could not build code\n");
    goto cleanup;
  }
  //dump_symbol((struct symbol *)ibin.sym.next_->data_);
  if (0 != build_pobj(outpath, ibin.bin, ibin.binsize, &ibin.sym, &ibin.symref)) {
    fprintf(stderr,"Could not build pobj\n");
    goto cleanup;
  }
  res = 0;
cleanup:
  list_clear(&tokens);
  if(ibin.bin) free(ibin.bin);
  list_clear(&ibin.sym);
  list_clear(&ibin.symref);
  return res;
}
