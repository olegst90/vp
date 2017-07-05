#include "memctrl.h"
#include <stdio.h>
#include "list.h"

struct memctrl_state {
  struct list_item devs;
};

struct memctrl_dev {
  struct memdev *dev;
  unsigned int addr;
  size_t len;
};

static struct memdev *memctrl_select(struct memdev *memctrl, const void *addr, size_t len, size_t *devoffset)
{
  struct memctrl_state *state = (struct memctrl_state *)memctrl->data;
  struct list_item *p = &state->devs;
  while (p = list_next(p)) {
    struct memctrl_dev *dev = (struct memctrl_dev *)list_data(p);
    if(addr >= dev->addr && addr + len < dev->addr + dev->len) {
      *devoffset = dev->addr;
      return dev->dev;
    }

  }
  return NULL;
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
  list_init(&state->devs);
  memctrl->data = state;
  memctrl->bulk_read = memctrl_bulk_read;
  memctrl->bulk_write = memctrl_bulk_write;
  memctrl->byte_read = memctrl_byte_read;
  memctrl->byte_write = memctrl_byte_write;
  return 0;
}

int memctrl_add_device(struct memdev *memctrl, struct memdev *dev, void *addr, size_t len)
{
  struct memctrl_state *state = (struct memctrl_state*)memctrl->data;
  struct memctrl_dev *new_dev = (struct memctrl_dev *)malloc(sizeof(*new_dev));
  new_dev->dev = dev;
  new_dev->addr = addr;
  new_dev->len = len;
  list_add_data(&state->devs, new_dev);
  return 0;
}
