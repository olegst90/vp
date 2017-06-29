#include "memctrl.h"
#include "ram.h"
#include <stdio.h>
#include "iosoc.h"

struct memctrl_state {
  struct memdev ram;
  struct memdev iosoc;
};

static struct memdev *memctrl_select(struct memdev *memctrl, const void *addr, size_t len, size_t *devoffset)
{
  struct memctrl_state *state = (struct memctrl_state *)memctrl->data;
  if (addr >= MEMCTRL_RAM_BASE && addr + len < MEMCTRL_RAM_BASE + MEMCTRL_RAM_RANGE) {
    printf("...selecting ram\n");
    *devoffset = MEMCTRL_RAM_BASE;
    return &state->ram;
  }
  if (addr >= MEMCTRL_IOSOC_BASE && addr + len < MEMCTRL_IOSOC_BASE + MEMCTRL_IOSOC_RANGE) {
    printf("...selecting iosoc\n");
    *devoffset = MEMCTRL_IOSOC_BASE;
    return &state->iosoc;
  }
}

static ssize_t memctrl_bulk_read(struct memdev *ctx, unsigned int offset, void *buffer, size_t size)
{
  size_t devoffset;
  struct memdev *dev = memctrl_select(ctx, offset, size, &devoffset);
  return dev->bulk_read(dev, offset - devoffset, buffer, size);
}

static ssize_t memctrl_bulk_write(struct memdev *ctx, unsigned int offset, const void *buffer, size_t size)
{
  size_t devoffset;
  struct memdev *dev = memctrl_select(ctx, offset, size, &devoffset);
  return dev->bulk_write(dev, offset - devoffset, buffer, size);
}

static int memctrl_byte_read(struct memdev *ctx, unsigned int offset, unsigned char *data)
{
  size_t devoffset;
  struct memdev *dev = memctrl_select(ctx, offset, 1, &devoffset);
  return dev->byte_read(dev, offset - devoffset, data);
}

static int memctrl_byte_write(struct memdev *ctx, unsigned int offset, unsigned char data)
{
  size_t devoffset;
  struct memdev *dev = memctrl_select(ctx, offset, 1, &devoffset);
  return dev->byte_write(dev, offset - devoffset, data);
}

int memctrl_init(struct memdev *memctrl)
{
  if (!memctrl) {
    fprintf(stderr, "%s bad param\n", __FUNCTION__);
    return -1;
  }
  struct memctrl_state *state = (struct memctrl_state *)calloc(sizeof(*state),1);
  ram_init(MEMCTRL_RAM_RANGE, &state->ram);
  iosoc_init(&state->iosoc);
  memctrl->data = state;
  memctrl->bulk_read = memctrl_bulk_read;
  memctrl->bulk_write = memctrl_bulk_write;
  memctrl->byte_read = memctrl_byte_read;
  memctrl->byte_write = memctrl_byte_write;
  return 0;
}
