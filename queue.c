#include "queue.h"

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

// bool queue_pop(struct queue *queue, unsigned int *popped_data) {}
