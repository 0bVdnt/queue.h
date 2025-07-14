#include "linked_list.h"

// Function pointers to (potentially) custom malloc() and
// free() functions.
static void *(*malloc_fptr)(size_t size) = NULL;
static void (*free_fptr)(void *addr) = NULL;

static struct node *create_node(unsigned int data) {
  if (malloc_fptr == NULL)
    return NULL;

  struct node *new_node = malloc_fptr(sizeof(struct node));
  if (new_node == NULL)
    return NULL;

  new_node->data = data;
  new_node->next = NULL;
  return new_node;
}

struct linked_list *linked_list_create(void) {
  if (malloc_fptr == NULL)
    return NULL;

  struct linked_list *ll = malloc_fptr(sizeof(struct linked_list));
  if (!ll)
    return NULL;

  ll->head = NULL;
  return ll;
}

bool linked_list_delete(struct linked_list *ll) {
  if (ll == NULL || free_fptr == NULL)
    return false;

  struct node *curr = ll->head;
  while (curr != NULL) {
    struct node *next = curr->next;
    free_fptr(curr);
    curr = next;
  }

  free_fptr(ll);
  return true;
}

size_t linked_list_size(struct linked_list *ll) {
  if (ll == NULL)
    return SIZE_MAX;
  size_t sz = 0;
  struct node *curr = ll->head;
  while (curr != NULL) {
    sz++;
    curr = curr->next;
  }

  return sz;
}

bool linked_list_insert_end(struct linked_list *ll, unsigned int data) {
  if (ll == NULL)
    return false;
  struct node *new_node = create_node(data);
  if (new_node == NULL)
    return false;
  if (ll->head == NULL) {
    ll->head = new_node;
  } else {
    struct node *curr = ll->head;
    while (curr->next != NULL)
      curr = curr->next;
    curr->next = new_node;
  }
  return true;
}

bool linked_list_insert_front(struct linked_list *ll, unsigned int data) {
  if (ll == NULL || malloc_fptr == NULL)
    return false;

  struct node *new_node = create_node(data);

  if (new_node == NULL)
    return false;

  new_node->next = ll->head;
  ll->head = new_node;

  return true;
}

bool linked_list_insert(struct linked_list *ll, size_t index,
                        unsigned int data) {
  if (ll == NULL || malloc_fptr == NULL)
    return false;

  if (index == 0)
    return linked_list_insert_front(ll, data);

  struct node *curr = ll->head;
  for (size_t i = 0; i < index - 1; i++) {
    if (curr == NULL) // Linked list too short
      return false;

    curr = curr->next;
  }

  if (curr == NULL)
    return false;

  struct node *new_node = create_node(data);
  if (new_node == NULL)
    return false;

  new_node->next = curr->next;
  curr->next = new_node;
  return true;
}

size_t linked_list_find(struct linked_list *ll, unsigned int data) {
  if (ll == NULL)
    return SIZE_MAX;

  size_t i = 0;
  struct node *curr = ll->head;
  while (curr != NULL) {
    if (curr->data == data)
      return i;

    curr = curr->next;
    i++;
  }
  return SIZE_MAX;
}

bool linked_list_remove(struct linked_list *ll, size_t index) {
  if (ll == NULL || ll->head == NULL || free_fptr == NULL)
    return false;
  struct node *curr = NULL;

  if (index == 0) {
    curr = ll->head;
    ll->head = ll->head->next;
  } else {
    struct node *prev = ll->head;
    for (size_t i = 0; i < index - 1; i++) {
      if (prev == NULL) // linked list is shorter than index
        return false;
      prev = prev->next;
    }

    if (prev == NULL || prev->next == NULL)
      return false;

    curr = prev->next;
    prev->next = curr->next;
  }
  free_fptr(curr);
  return true;
}

struct iterator *linked_list_create_iterator(struct linked_list *ll,
                                             size_t index) {
  if (ll == NULL || malloc_fptr == NULL)
    return NULL;
  struct iterator *itr = malloc_fptr(sizeof(struct iterator));
  if (!itr)
    return NULL;

  struct node *curr = ll->head;
  for (size_t i = 0; i < index; i++) {
    if (curr == NULL) {
      free_fptr(itr);
      return NULL;
    }
    curr = curr->next;
  }

  if (curr == NULL) {
    free_fptr(itr);
    return NULL;
  }

  itr->ll = ll;
  itr->current_index = index;
  itr->current_node = curr;
  itr->data = curr->data;

  return itr;
}

bool linked_list_delete_iterator(struct iterator *iter) {
  if (iter == NULL || free_fptr == NULL)
    return false;
  free_fptr(iter);
  return true;
}

bool linked_list_iterate(struct iterator *iter) {
  if (iter == NULL || iter->ll == NULL || iter->current_node == NULL ||
      iter->current_node->next == NULL)
    return false;

  iter->current_node = iter->current_node->next;
  iter->data = iter->current_node->data;
  iter->current_index++;
  return true;
}

bool linked_list_register_malloc(void *(*malloc_f)(size_t)) {
  if (malloc_f == NULL)
    return false;
  malloc_fptr = malloc_f;
  return true;
}

bool linked_list_register_free(void (*free_f)(void *)) {
  if (free_f == NULL)
    return false;
  free_fptr = free_f;
  return true;
}
