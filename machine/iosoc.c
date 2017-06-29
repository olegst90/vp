#include "iosoc.h"
#include <stdio.h>

struct iosoc_state {
  int a,b,c;
};

static int iosoc_byte_read(struct memdev *ctx, unsigned int offset, unsigned char *data)
{
  if (!ctx || !ctx->data || !data) {
    fprintf(stderr, "%s bad param\n", __FUNCTION__);
    return -1;
  }

  return 0;
}

static int iosoc_byte_write(struct memdev *ctx, unsigned int offset, unsigned char data)
{
  if (!ctx || !ctx->data) {
    fprintf(stderr, "%s bad param\n", __FUNCTION__);
    return -1;
  }
  //printf("%s writing %c to %p", __FUNCTION__, data, offset);
  switch (offset) {
    case IOSOC_OFFSET_TX:
    putchar((char)data);
    break;
  }
  return 0;
}


int iosoc_init(struct memdev *dev)
{
  if (!dev) {
    fprintf(stderr, "%s bad param\n", __FUNCTION__);
    return -1;
  }
  dev->data = calloc(sizeof(struct iosoc_state), 1);
  dev->byte_read = iosoc_byte_read;
  dev->byte_write = iosoc_byte_write;
  return 0;
}
