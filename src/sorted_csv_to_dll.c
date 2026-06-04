#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "setting_up.h"

// Function to create a new node for the doubly linked list
student_node* create_node(student_node student) {
    student_node* new_node = (student_node*)malloc(sizeof(student_node));
    if (new_node) {
        *new_node = student;  // Copy the student data to the new node
        new_node->next = NULL;
        new_node->prev = NULL;
    }
    return new_node;
}

// Function to insert a node at the end of the doubly linked list
void insert_end(student_node_dll *list, student_node student) {
    student_node* new_node = create_node(student);
    if (list->head == NULL) {
        list->head = new_node;  // If the list is empty, make the new node the head
    } else {
        student_node* temp = list->head;
        while (temp->next != NULL) {
            temp = temp->next;  // Traverse to the end of the list
        }
        temp->next = new_node;  // Insert new node at the end
        new_node->prev = temp;   // Set the previous pointer
    }
}

// Function to print the doubly linked list
void print_list(student_node_dll *list) {
    student_node* temp = list->head;
    printf("Doubly Linked List Contents:\n");
    while (temp != NULL) {
        printf("Merit No: %u, Name: %s, MIS: %llu, Branch: %s, Category: %s, CGPA: %.2f, Weighted CGPA: %.2f, Room No: %d\n",
               temp->merit_no, temp->name, temp->mis, temp->branch, temp->category, temp->cgpa, temp->weighted_cgpa, temp->room_no);
        temp = temp->next;  // Move to the next node
    }
}

// Function to create a doubly linked list from a sorted CSV file
student_node_dll create_dll_from_csv(const char *filename) {
    student_node_dll list = { NULL };  // Initialize the DLL head to NULL
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open file");
        return list;  // Return an empty list if file cannot be opened
    }

    student_node student;
    // Read the CSV file line by line
    while (fscanf(file, "%u,%99[^,],%llu,%14[^,],%14[^,],%f,%d\n",
                  &student.merit_no, student.name, &student.mis,
                  student.branch, student.category, &student.weighted_cgpa, &student.room_no) == 7) {
        insert_end(&list, student);  // Insert each student node at the end of the list
    }

    fclose(file);
    return list;  // Return the populated doubly linked list
}

