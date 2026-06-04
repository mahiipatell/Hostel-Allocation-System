#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include "setting_up.h"

// Define the structure for a queue node (which will hold a student_node)
typedef struct queue_node {
    student_node *student;          // Pointer to the student_node
    struct queue_node *next;        // Pointer to the next queue_node
    struct queue_node *prev;        // Pointer to the previous queue_node (doubly linked)
} queue_node;

// Define the queue structure itself
typedef struct {
    queue_node *front;  // Pointer to the front node of the queue
    queue_node *rear;   // Pointer to the rear node of the queue
} queue;

// Function prototypes
void init_queue(queue *q);
bool enqueue(queue *q, student_node *value);
student_node* dequeue(queue *q, bool *success);
bool is_empty_queue(queue *q);
void print_queue(queue *q);

#endif // QUEUE_H
