#include "queue.h"
#include "linked_list.h"

// Function pointers to (potentially) custom malloc() and
// free() functions.

static void *(*malloc_fptr)(size_t size) = NULL;
static void (*free_fptr)(void *addr) = NULL;

struct queue *queue_create(void) {
  if (malloc_fptr == NULL)
    return NULL;

  struct queue *q = malloc_fptr(sizeof(struct queue));
  if (q == NULL)
    return NULL;

  q->ll = linked_list_create();
  if (q->ll == NULL) {
    free_fptr(q);
    return NULL;
  }
  return q;
}

bool queue_delete(struct queue *queue) {
  if (queue == NULL || free_fptr == NULL)
    return false;

  linked_list_delete(queue->ll);
  free_fptr(queue);
  return true;
}

bool queue_push(struct queue *queue, unsigned int data) {
  if (queue == NULL)
    return false;
  // Push is O(1) due to the tail pointer
  return linked_list_insert_end(queue->ll, data);
}

bool queue_pop(struct queue *queue, unsigned int *popped_data) {
  if (queue == NULL || linked_list_size(queue->ll) == 0)
    return false;
  *popped_data = queue->ll->head->data;
  return linked_list_remove(queue->ll, 0); // O(1) remove from head operation
}

size_t queue_size(struct queue *queue) {
  if (queue == NULL || queue->ll == NULL)
    return SIZE_MAX;
  return linked_list_size(queue->ll);
}

bool queue_has_next(struct queue *queue) {
  if (queue == NULL || queue->ll == NULL)
    return false;

  return (linked_list_size(queue->ll) > 0);
}

bool queue_next(struct queue *queue, unsigned int *popped_data) {
  if (queue == NULL || queue->ll == NULL || queue->ll->head == NULL)
    return false;
  *popped_data = queue->ll->head->data;
  return true;
}

bool queue_register_malloc(void *(*malloc)(size_t)) {
  if (!linked_list_register_malloc(malloc))
    return false;
  malloc_fptr = malloc;
  return true;
}

bool queue_register_free(void (*free)(void *)) {
  if (!linked_list_register_free(free))
    return false;
  free_fptr = free;
  return true;
}
