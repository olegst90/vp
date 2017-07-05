#ifndef LIST_H
#define LIST_H

struct list_item {
  struct list_item *next_;
  void *data_;
};

#ifndef l_free
#define l_free free
#endif
#ifndef l_malloc
#define l_malloc malloc
#endif

#define list_init(head) do { (head)->next_ = NULL; (head)->data_ = NULL; } while (0);
#define list_first(head) (head->next_)

#define list_new_item(data) ({\
  struct list_item *item = (struct list_item *)l_malloc(sizeof(struct list_item));\
  item->data_ = (data);\
  item->next_ = NULL;\
  item;\
})

#define list_add(head,item) do { \
  struct list_item *p = (head);\
  while (p->next_) p = p->next_;\
  p->next_ = item;\
} while (0);

#define list_add_data(head,data) do { \
  struct list_item *p = (head);\
  while (p->next_) p = p->next_;\
  p->next_ = (struct list_item *)l_malloc(sizeof(struct list_item));\
  p->next_->next_ = NULL;\
  p->next_->data_ = data;\
} while (0);

#define list_find_item(head,item) ({\
  struct list_item *p = NULL;\
  for(p = head; p && p != item; p = p->next_);\
  (p);\
})

#define list_find_data(head,data) ({\
  struct list_item *p = NULL;\
  void *pdata = NULL;\
  for(p = head; p && p->data_ != data; p = p->next_);\
  if (p) pdata = p->data_;\
  pdata;\
})

#define list_remove(head, item) do {\
  struct list_item *tmp;\
  while (head->next_ != item && head->next_) head = head->next_;\
  if (head->next_){\
    tmp = head->next_;\
    head->next_ = head->next_->next_;\
    l_free(tmp->data_);\
    l_free(tmp);\
  }\
} while(0)

#define list_clear(head) do {\
  struct list_item *todelete = NULL;\
  struct list_item *p = (head)->next_;\
  while (p) {\
    if(todelete) l_free(todelete);\
    l_free(p->data_);\
    todelete = p;\
    p = p->next_;\
  }\
  if(todelete) l_free(todelete);\
} while (0)

#define list_next(item) (item->next_)

#define list_data(item) (item->data_)

#endif
