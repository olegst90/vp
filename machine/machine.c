#include <stdio.h>
#include "memdev.h"
#include "cpu.h"
#include "cpu_default.h"
#include "memctrl.h"

#define RAM_BASE 0
#define RAM_RANGE 64

#define IOSOC_BASE 0x1000
#define IOSOC_RANGE 0x1000

struct machine {
  struct memdev mem;
  struct cpu cp;
};

int main(int argc, char **argv)
{
  struct machine m;
  puts("memctrl init");
  memctrl_init(&m.mem);
  puts("ram init");
  struct memdev *ram = (struct memdev *)malloc(sizeof(*ram));
  ram_init(ram, RAM_RANGE);
  memctrl_add_device(&m.mem, ram, RAM_BASE, RAM_RANGE);
  puts("iosoc init");
  struct memdev *iosoc = (struct memdev *)malloc(sizeof(*iosoc));
  iosoc_init(iosoc, IOSOC_RANGE);
  memctrl_add_device(&m.mem, iosoc, IOSOC_BASE, IOSOC_RANGE);
  puts("cpu init");
  cpu_default_init(&m.mem, &m.cp);
  puts("starting");
  while(1) {
    m.cp.cycle(&m.cp);
    getchar();
  }
  return 0;
}
