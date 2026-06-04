#include <stdio.h>
#include <stdlib.h>
#include "setting_up.h"
#include "stack.h"


// Function to initialize the stack with a given size
void init_stack(stack* s, int size) {
    s->student_stack = (student_node*) malloc(size * sizeof(student_node));  // Allocate memory for student array
    if (s->student_stack == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    s->top = -1;
    s->max_size = size;
}

// Function to check if the stack is full, return 1 if full 0 if empty
int is_stack_full(stack* s) {
    if(s->top == s->max_size - 1){
        return 1;
    }
    return 0;
}

// Function to check if the stack is empty, return 1 if empty 0 if full
int is_empty_stack(stack* s) {
    if(s->top == -1){
        return 1;
    }
    return 0;
}

// Function to push a student onto the stack
void push(stack* s, student_node sn) {
        s->student_stack[++(s->top)] = sn;  // Add student to stack
}

// Function to print the contents of the stack
void print_stack(stack s) {
    if (is_empty_stack(&s)) {
        printf("The stack is empty.\n");
        return;
    }

    printf("Stack contents (from top to bottom):\n");
    for (int i = s.top; i >= 0; i--) {
        student_node sn = s.student_stack[i];
        printf("Merit No: %u, Name: %s\n",
               sn.merit_no, sn.name);
    }
}
