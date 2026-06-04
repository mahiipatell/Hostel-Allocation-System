#ifndef HASH_H
#define HASH_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "setting_up.h"  // Assuming this contains the definition of student_node
#include "linked_list.h"

// Structure for hash table
typedef struct hash_table {
    student_node** table;   // Array of pointers to student nodes (linked lists)
    int size;               // Size of the hash table, total number of rooms available
    int floors;
    int rooms_per_floor;
} hash_table;

// Function prototypes

// Create and initialize the hash table
hash_table* create_hash_table(int size, int floors, int rooms_per_floor);

// Hash function to compute index from room number
int hash_function(int room_number, int floors, int rooms_per_floor);

// Reverse hash function to get room number from index
int reverse_hash_function(int index, int rooms_per_floor);

// Function to print all room numbers and student names
void print_hash(hash_table* ht);

// Function to search for students by merit number and print their room numbers
bool search_by_merit_no(hash_table* fix, hash_table* ht, unsigned int merit_no);

// Function to append a student node to the hash table at a given index (doubly linked list)
int append_to_hash(hash_table* ht, int index, student_node* temp_node);

// Function to remove nodes with a specific merit number from the hash table (handles multiple occurrences)
void remove_from_hash(hash_table* ht, unsigned int merit_no);

// Function to print all empty rooms (indexes pointing to NULL) in the hash table
void print_empty_rooms(hash_table* ht);

//function to print rooms having less than 3 nodes
void print_incomplete_rooms(hash_table *ht);

//function to check if all rooms are at capacity
int all_rooms_full(hash_table *ht);

#endif  // HASH_H
