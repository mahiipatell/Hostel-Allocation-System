#include "linked_list.h"

//initialises a doubly linked list of students
void init_dll(student_node_dll *dlls) {
    dlls->head = NULL;
    dlls->tail = NULL;
    return;
}

//adds a node to the end of the doubly linked list of students
void insert_end_dll(student_node_dll *dlls, student_node sn) {
    student_node *new;
    new = (student_node *)malloc(sizeof(student_node));
    if (!(dlls->head)) {
        dlls->head = new;
        dlls->tail = new;
        new->prev = NULL;
    } else {
        dlls->tail->next = new;
        new->prev = dlls->tail;
        dlls->tail = new;
    }
    *new = sn;
    new->next = NULL;
    return;
}

//removes a particular node (mainly would be used for removing a node from hold hash table) 
//and returns the removed node (to enter in fixed hash table)
student_node *remove_student_node(student_node_dll *dlls, student_node *sn) {
    if (sn == NULL) {
        return NULL; 
    }
    if (sn == dlls->head) {
        dlls->head = sn->next;
        if (dlls->head != NULL) {
            dlls->head->prev = NULL;
        }
    }
    if (sn == dlls->tail) {
        dlls->tail = sn->prev;
        if (dlls->tail != NULL) {
            dlls->tail->next = NULL;
        }
    }
    if (sn->prev != NULL) {
        sn->prev->next = sn->next;
    }
    if (sn->next != NULL) {
        sn->next->prev = sn->prev;
    }
    return sn; 
}

//returns the student node based on mis (used for searching if a student is on hold)
student_node *search_student(student_node_dll *dlls, unsigned long long int mis) {
    student_node *current = dlls->head;
    while (current != NULL) {
        if (current->mis == mis) {
            return current; 
        }
        current = current->next;
    }
    return NULL; // Return NULL if student not found
}

// Function to create and return a new student node based on merit number
student_node *info_from_merit_no(student_node_dll *dlls, unsigned int merit_no) {
    student_node *current = dlls->head;
    
    // Search for the student node with the given merit number
    while (current != NULL) {
        if (current->merit_no == merit_no) {
            // Create a new student node and copy data from the found node
            student_node *new_node = (student_node *)malloc(sizeof(student_node));
            *new_node = *current;  // Copying all fields from current to new node
            new_node->next = NULL;  // New node will be a standalone node
            new_node->prev = NULL;
            
            // Return the new student node
            return new_node;
        }
        current = current->next;  // Move to the next node
    }

    // If student with given merit number is not found, return NULL
    printf("Student with Merit No: %u not found.\n", merit_no);
    return NULL;
}
