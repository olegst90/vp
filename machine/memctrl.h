#ifndef MEMCTRL_H
#define MEMCTRL_H

#include "memdev.h"

int memctrl_init(struct memdev *memctrl);
int memctrl_add_device(struct memdev *memctrl, struct memdev *dev, void *addr, size_t len);


#endif
