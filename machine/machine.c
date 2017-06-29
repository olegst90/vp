#include <stdio.h>
#include "memdev.h"
#include "cpu.h"
#include "cpu_default.h"
#include "memctrl.h"

struct machine {
  struct memdev mem;
  struct cpu cp;
};

int main(int argc, char **argv)
{
  struct machine m;
  puts("memctrl init");
  memctrl_init(&m.mem);
  puts("cpu init");
  cpu_default_init(&m.mem, &m.cp);
  puts("starting");
  while(1) {
    m.cp.cycle(&m.cp);
    getchar();
  }
  return 0;
}
