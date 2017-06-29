#ifndef MEMCTRL_H
#define MEMCTRL_H

#include "memdev.h"

#define MEMCTRL_RAM_BASE 0
#define MEMCTRL_RAM_RANGE 64

#define MEMCTRL_IOSOC_BASE 0x1000
#define MEMCTRL_IOSOC_RANGE 0x1000

int memctrl_init(struct memdev *memctrl);

#endif
