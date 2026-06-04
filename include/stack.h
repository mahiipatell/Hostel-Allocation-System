#include "setting_up.h"
// Stack structure that will hold how many spaces for each branch and category
typedef struct stack {
    student_node* student_stack;  // Pointer to dynamically allocated array of students
    int top;                   // Index of the top element in the stack
    int max_size;              // Maximum size of the stack
}stack;

void init_stack(stack* s, int size);
int is_stack_full(stack* s);
int is_empty_stack(stack* s);
void push(stack* s, student_node sn);
void print_stack(stack s);