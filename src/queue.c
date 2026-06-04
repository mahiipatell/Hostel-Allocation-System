#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

// Initialize the queue
void init_queue(queue *q) {
    q->front = NULL;
    q->rear = NULL;
}

// Check if the queue is empty
bool is_empty_queue(queue *q) {
    return q->front == NULL;
}

// Add an element to the queue (enqueue)
bool enqueue(queue *q, student_node *value) {
    // Create a new queue node
    queue_node *new_node = (queue_node *)malloc(sizeof(queue_node));
    if (new_node == NULL) {
        printf("Memory allocation failed. Cannot enqueue.\n");
        return false;
    }
    
    new_node->student = value;
    new_node->next = NULL;
    new_node->prev = q->rear;  // New node's previous is the current rear

    if (is_empty_queue(q)) {
        // If the queue is empty, the new node is both front and rear
        q->front = new_node;
    } else {
        // Otherwise, link the current rear to the new node
        q->rear->next = new_node;
    }

    // Update the rear to the new node
    q->rear = new_node;

    return true;
}

// Remove an element from the queue (dequeue)
student_node* dequeue(queue *q, bool *success) {
    if (is_empty_queue(q)) {
        printf("Queue is empty. Cannot dequeue.\n");
        *success = false;
        return NULL;
    }

    // Get the front node
    queue_node *temp = q->front;
    student_node *value = temp->student;

    // Move the front pointer to the next node
    q->front = q->front->next;

    if (q->front == NULL) {
        // If the queue is now empty, update the rear as well
        q->rear = NULL;
    } else {
        // Otherwise, update the new front node's previous pointer
        q->front->prev = NULL;
    }

    // Free the old front node
    free(temp);

    *success = true;
    return value;
}

// Function to print the contents of the queue
void print_queue(queue *q) {
    if (is_empty_queue(q)) {
        printf("The queue is empty.\n");
        return;
    }

    printf("Queue contents (from front to rear):\n");
    queue_node *current = q->front;
    while (current != NULL) {
        student_node *sn = current->student;
        printf("Merit No: %u, Name: %s\n",
               sn->merit_no, sn->name);
        current = current->next;  // Move to the next node
    }
}
