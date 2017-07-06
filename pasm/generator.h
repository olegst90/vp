#ifndef GENERATOR_H
#define GENERATOR_H

#include <list.h>
#include <stdlib.h>
#include <stdint.h>

/* intermediate binary file */
struct interbin {
  uint8_t *bin;
  uint32_t binsize;
  struct list_item sym;
  struct list_item symref;
};

int build_interbin(struct list_item *tokens, struct interbin *bin);

#endif //GENERATOR_H
