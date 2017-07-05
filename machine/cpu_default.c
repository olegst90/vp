#include "cpu_default.h"
#include <stdio.h>
#include "iosoc.h"
#include "memctrl.h"
#include <stdint.h>

struct cpu_default {
  int16_t A,I;
  int8_t flags;
  unsigned int pc;
};

static int fetch_instruction(void *mem, int *len, int16_t *v1, int16_t *v2)
{
  uint8_t code = *(uint8_t *)mem;
  switch(code){
    CPU_DEFAULT_MV_IMM_A:
    CPU_DEFAULT_MV_IMM_I:
    CPU_DEFAULT_ADD_IMM_A:
    CPU_DEFAULT_ADD_IMM_I:
    CPU_DEFAULT_SUB_IMM_A:
    CPU_DEFAULT_SUB_IMM_I:
    CPU_DEFAULT_MUL_IMM_A:
    CPU_DEFAULT_MUL_IMM_I:
    CPU_DEFAULT_DIV_IMM_A:
    CPU_DEFAULT_DIV_IMM_I:
    CPU_DEFAULT_JMP:
    CPU_DEFAULT_JMPZ:
    CPU_DEFAULT_JMPN:
    CPU_DEFAULT_JMPC:
      *v1 = *(int16_t *)(mem + 1);
  }
}

static int cpu_default_cycle(struct cpu *p)
{
  //printf("%s enter\n", __FUNCTION__);
  struct cpu_default *defcpu = (struct cpu_default *)p->data;



  p->memctrl->byte_write(p->memctrl, IOSOC_BASE + IOSOC_OFFSET_TX,'c');
  printf("A: %#x | I: %#x | pc: %#x\n", defcpu->A, defcpu->I, defcpu->pc);
  defcpu->pc++;
  return 0;
}

int cpu_default_init(struct memdev *memctrl, struct cpu *p)
{
  if (!p || !MEMDEV_VALID(memctrl)) {
    fprintf(stderr, "%s bad param\n", __FUNCTION__);
    return -1;
  }
  p->memctrl = memctrl;
  struct cpu_default *state = (struct cpu_default *)calloc(sizeof(struct cpu_default), 1);
  if (!state) {
    fprintf(stderr, "%s can't allocate data\n", __FUNCTION__);
    return -1;
  }
  p->data = state;
  p->cycle = cpu_default_cycle;
  return 0;
}
