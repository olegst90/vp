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
  build_interbin(&tokens, &ibin);

cleanup:
  list_clear(&tokens);
  free(ibin.bin);
  list_clear(&ibin.sym);
  list_clear(&ibin.symref);
  //list_clear(&out.sym);
  //list_clear(&out.symref);
  //if (infile) fclose(infile);
  //if (outfile) fclose(outfile);
  return res;
}
