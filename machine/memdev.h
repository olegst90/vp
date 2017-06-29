#ifndef MEMDEV_H
#define MEMDEV_H

#include <stdlib.h>
//#include <sys/types.h>

struct memdev;

typedef ssize_t (*bulk_read_fp)(struct memdev *, unsigned int, void *, size_t);
typedef ssize_t (*bulk_write_fp)(struct memdev *, unsigned int, const void *, size_t);
typedef int (*byte_read_fp)(struct memdev *, unsigned int, unsigned char *);
typedef int (*byte_write_fp)(struct memdev *, unsigned int, unsigned char);

struct memdev {
  void *data;
  bulk_read_fp bulk_read;
  bulk_write_fp bulk_write;
  byte_read_fp byte_read;
  byte_write_fp byte_write;
};

#define MEMDEV_VALID(dev) (dev && dev->data && \
                           dev->bulk_read && dev->bulk_write && \
                           dev->byte_read && dev->byte_write)

#endif
