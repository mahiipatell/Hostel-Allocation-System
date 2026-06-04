#ifndef SETTING_UP_H
#define SETTING_UP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*contains file to sorted csv
sorted csv to dll
and logistic*/


// Constants
#define MAX_STUDENTS 1000
//#define OUTPUT_FILE "sorted_students.csv"

// Structure definition for student node
typedef struct student_node {
    unsigned int merit_no;  // Merit number assigned after sorting
    char name[100];         // Student name
    unsigned long long int mis;  // MIS number
    char branch[15];        // Branch of study
    char category[15];      // Category of the student (e.g., reserved, open)
    float cgpa;             // Cumulative Grade Point Average
    float weighted_cgpa;    // Weighted CGPA based on statistical calculations
    int room_no;            // Room number (initialized to -1)
    struct student_node *next;  // Pointer to the next node (for a linked list, if needed)
    struct student_node *prev;  // Pointer to the previous node (for a linked list, if needed)
} student_node;

// Structure of dll of students
typedef struct student_node_dll {
    student_node *head;     // Head node of the doubly linked list
    student_node *tail; // Tail node of the doubly linked list
} student_node_dll;


// Function declarations for file to sorted csv
void calculate_statistics(student_node *students, int count, const char *branch, float *mean, float *stddev);
int read_students(const char *filename, student_node *students);
void calculate_weighted_cgpa(student_node *students, int count);
int compare_students(const void *a, const void *b);
void write_students(const char *filename, student_node *students, int count);
void input_logistics(int logistics[]);

//Function declaration sorted csv to dll
student_node* create_node(student_node student);            // Create a new node
void insert_end(student_node_dll *list, student_node student); // Insert a node at the end of the DLL
void print_list(student_node_dll *list);                    // Print the list
student_node_dll create_dll_from_csv(const char *filename); // Create a list from a CSV file

#endif  // SETTING_UP_H
