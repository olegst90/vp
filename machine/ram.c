#include "memdev.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static ssize_t ram_bulk_read(struct memdev *ctx, unsigned int offset, void *buffer, size_t size)
{
  if (!buffer || !ctx || !ctx->data) {
    fprintf(stderr, "%s bad param\n", __FUNCTION__);
    return -1;
  }
  memcpy(buffer, ctx->data + offset, size);
  return size;
}

static ssize_t ram_bulk_write(struct memdev *ctx, unsigned int offset, const void *buffer, size_t size)
{
  if (!buffer || !ctx || !ctx->data) {
    fprintf(stderr, "%s bad param\n", __FUNCTION__);
    return -1;
  }
  memcpy(ctx->data + offset, buffer, size);
  return size;
}

static int ram_byte_read(struct memdev *ctx, unsigned int offset, unsigned char *data)
{
  if (!ctx || !ctx->data || !data) {
    fprintf(stderr, "%s bad param\n", __FUNCTION__);
    return -1;
  }
  *data = *(unsigned char *)(ctx->data);
  return 1;
}

static int ram_byte_write(struct memdev *ctx, unsigned int offset, unsigned char data)
{
  if (!ctx || !ctx->data) {
    fprintf(stderr, "%s bad param\n", __FUNCTION__);
    return -1;
  }
  *(unsigned char *)(ctx->data) = data;
  return 1;
}

int ram_init(size_t len, struct memdev *ram)
{
   if (!ram) {
     fprintf(stderr, "%s bad param\n", __FUNCTION__);
     return -1;
   }
   ram->data = calloc(len, 1);
   if (!ram->data) {
     fprintf(stderr, "%s can't allocate ram\n", __FUNCTION__);
     return -1;
   }
   ram->bulk_read = ram_bulk_read;
   ram->bulk_write = ram_bulk_write;
   ram->byte_read = ram_byte_read;
   ram->byte_write = ram_byte_write;
   return 0;
}

void ram_destroy(struct memdev *ram)
{
  if (ram) {
    free(ram->data);
    memset(ram, 0, sizeof(*ram));
  }
}
