#include <stdio.h>
#include <stdlib.h>
#define l_free myfree
#define l_malloc mymalloc
#include "../aux/list.h"

struct ds {
  int i;
};
void myfree(void *p) {
  printf("free %p\n", p);
  free(p);
}

void *mymalloc(size_t len) {
  void *p = malloc(len);
  printf("%p:%zd allocated\n",p, len);
  return p;
}
int main() {
  struct list_item head;
  list_init(&head);
  struct ds *d1 = (struct ds *)l_malloc(sizeof(struct ds));
  struct ds *d2 = (struct ds *)l_malloc(sizeof(struct ds));
  list_add_data(&head, d1);
  list_add_data(&head, d2);

  d2->i = 7;
  struct ds *p = list_find_data(&head, d2);
  printf("%d\n", p->i);
  list_clear(&head);
  return 0;
}
