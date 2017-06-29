#ifndef CPU_H
#define CPU_H

#include "memdev.h"

struct cpu;

typedef int(*cpu_cycle_fp)(struct cpu *);

struct cpu {
  void *data;
  struct memdev *memctrl;
  cpu_cycle_fp cycle;
};

#endif
