#include "../include/setting_up.h"
#include "../include/hash.h"
#include "../include/linked_list.h"

// Function to create and initialize the hash table, size is total number of rooms 
hash_table* create_hash_table(int size) {
    // Allocate memory for the hash table structure
    hash_table* ht = (hash_table*)malloc(sizeof(hash_table));
    if (ht == NULL) {
        printf("Memory allocation failed for hash table.\n");
        return NULL;  // Return NULL if allocation fails
    }

    ht->size = size;  // Set the size of the hash table

    // Allocate memory for the table array (array of pointers)
    ht->table = (student_node**)malloc(size * sizeof(student_node*));
    if (ht->table == NULL) {
        printf("Memory allocation failed for hash table array.\n");
        free(ht);  // Free the hash table structure before returning
        return NULL;
    }

    // Initialize all elements of the table array to NULL
    for (int i = 0; i < size; i++) {
        ht->table[i] = NULL;
    }

    return ht;  // Return the initialized hash table
}


// Hash function to compute index based on room number
int hash_function(int room_number, int table_size) {
    if (room_number >= 101 && room_number <= 120) {
        return (room_number - 101) % table_size;  // Map to bucket 0-19, rooms 101 - 120
    } else if (room_number >= 201 && room_number <= 220) {
        return (room_number - 201 + 20) % table_size;  // Map to bucket 20-39, rooms 201 - 220
    } else if (room_number >= 301 && room_number <= 320) {
        return (room_number - 301 + 40) % table_size;  // Map to bucket 40-59, rooms 301 - 320
    }
    return -1;  // Return -1 for invalid room numbers
}

// Reverse hash function to retrieve room number from index
int reverse_hash_function(int index) {
    int room_number;

    if (index >= 0 && index < 20) {
        room_number = 101 + index;  // Rooms 101-120
    } else if (index >= 20 && index < 40) {
        room_number = 201 + (index - 20);  // Rooms 201-220
    } else if (index >= 40 && index < 60) {
        room_number = 301 + (index - 40);  // Rooms 301-320
    } else {
        room_number = -1;  // Invalid index
    }
    return room_number;
}

// Function to print room numbers and student names
void print_hash(hash_table* ht) {
    for (int i = 0; i < ht->size; i++) {
        if (ht->table[i] != NULL) {  // Check if the bucket is not empty
            student_node* current = ht->table[i];  // Start at the head of the linked list
            
            while (current != NULL) {  // Traverse the linked list
                int room_number = reverse_hash_function(i);
                if (room_number != -1) {
                    printf("  Room %d: %s\n", room_number, current->name);  // Print room number and student name
                }
                current = current->next;  // Move to the next node in the list
            }
        }
    }
}


// Function to search for students by merit number and print their room numbers
// Returns true if any students are found, false if none are found
bool search_by_merit_no(hash_table* fix, hash_table* ht, unsigned int merit_no) {
    bool found = false;  // To track if any students are found
    //printf("Searching for students with Merit No: %u\n", merit_no);
    int current_index;
    // Traverse the entire hash table to find matching merit_no
    for (int i = 0; i < ht->size; i++) {
        current_index = i;
        student_node* current = ht->table[i];  // Get the head of the linked list at this index

        // Traverse the linked list at this index
        while (current != NULL) {
            if (current->merit_no == merit_no) {
                // Print the room number when a match is found
                int room_no = reverse_hash_function(current_index);
                // find person who allotted you
                printf("%d by %s\n", room_no, (fix->table[current_index])->name);
                found = true;
            }
            current = current->next;  // Move to the next node in the linked list
        }
    }

    if (!found) {
       // printf("No previous allocation\n");
        return false;  // Return false if no students are found
    }

    return true;  // Return true if at least one student was found
}


// Function to append a student node to the hash table at a given index (doubly linked list)
// Function to append a student node to the hash table using merit number and index
// Function to append a student node to the hash table at a given index (doubly linked list)
int append_to_hash(hash_table* ht, int index, student_node* temp_node) {
    // Check if the index is valid
    if (index < 0 || index >= ht->size) {
        printf("Error: Invalid index %d for hash table of size %d\n", index, ht->size);
        return -1;
    }

    // If the bucket at the index is empty, directly assign the temp_node
    if (ht->table[index] == NULL) {
        ht->table[index] = temp_node;
        temp_node->next = NULL;
        temp_node->prev = NULL;
    } else {
        // If the bucket is not empty, traverse to the end of the linked list
        student_node* current = ht->table[index];

        // Traverse to the last node in the doubly linked list
        while (current->next != NULL) {
            current = current->next;
        }

        // Append the new node at the end of the list
        current->next = temp_node;
        temp_node->prev = current;
        temp_node->next = NULL;
    }

    // Debug message for confirmation
   // printf("Student '%s' with Merit No: %u has been added to the hash table\n",
           //temp_node->name, temp_node->merit_no);
    return 1;
}


// Function to remove nodes with a specific merit number from the hash table
void remove_from_hash(hash_table* ht, unsigned int merit_no) {
    bool found = false;  // To track if any nodes are found and removed

    // Traverse the entire hash table
    for (int i = 0; i < ht->size; i++) {
        student_node* current = ht->table[i];
        student_node* to_delete = NULL;

        // Traverse the linked list at this index
        while (current != NULL) {
            // Check if the current node has the specified merit number
            if (current->merit_no == merit_no) {
                found = true;
                //printf("Removing student '%s' with Merit No: %u from Room No: %d\n",
                    //   current->name, current->merit_no, current->room_no);

                // Update pointers to remove the current node
                if (current->prev != NULL) {
                    current->prev->next = current->next;
                } else {
                    // If current is the head of the list, update the head pointer
                    ht->table[i] = current->next;
                }

                if (current->next != NULL) {
                    current->next->prev = current->prev;
                }

                // Save the node to delete and move to the next node
                to_delete = current;
                current = current->next;

                // Free the memory of the deleted node
                free(to_delete);
            } else {
                // Move to the next node if merit number doesn't match
                current = current->next;
            }
        }
    }

   /* if (!found) {
        printf("No student found with Merit No: %u\n", merit_no);
    } else {
        printf("All occurrences of Merit No: %u have been removed from the hash table.\n", merit_no);
    }*/
}


// Function to print empty rooms in a floor-wise format

// Function to print all empty rooms in the specified format
void print_empty_rooms(hash_table* ht) {
    //printf("List of empty rooms:\n");

    // Room ranges for each floor
    int floors[][2] = {
        {101, 120}, // Floor 1: Room 101 - 120
        {201, 220}, // Floor 2: Room 201 - 220
        {301, 320}  // Floor 3: Room 301 - 320
    };

    // Flag to check if any empty room exists
    bool any_empty = false;

    // Traverse each floor
    for (int f = 0; f < 3; f++) {
        int start = floors[f][0]; // Start room number
        int end = floors[f][1];   // End room number

        // Print header for the floor
        printf("\nFloor %d:\n", (start / 100));

        // Loop through the rooms in the floor
        for (int room = start; room <= end; room++) {
            int index = hash_function(room, ht->size);

            // If the room is empty, print its number
            if (ht->table[index] == NULL) {
                printf("%d  ", room);
                any_empty = true;
            } else {
                printf("     "); // Print spaces for occupied rooms
            }

            // Add a newline after every 10 rooms
            if ((room % 10) == 0) {
                printf("\n");
            }
        }
    }

    // If no empty rooms were found, print a message
    if (!any_empty) {
        printf("\nNo empty rooms available.\n");
    }
}


void print_incomplete_rooms(hash_table *ht) {
    // Room ranges for each floor
    int floors[][2] = {
        {101, 120}, // Floor 1: Room 101 - 120
        {201, 220}, // Floor 2: Room 201 - 220
        {301, 320}  // Floor 3: Room 301 - 320
    };

    // Flag to check if any incomplete room exists
    bool any_incomplete = false;

    // Traverse each floor
    for (int f = 0; f < 3; f++) {
        int start = floors[f][0]; // Start room number
        int end = floors[f][1];   // End room number

        // Print header for the floor
        printf("\nFloor %d (Rooms with less than 3 students):\n", (start / 100));

        // Loop through the rooms in the floor
        for (int room = start; room <= end; room++) {
            int index = hash_function(room, ht->size);
            student_node *current = ht->table[index];
            int count = 0;

            // Count the number of nodes (students) at this index
            while (current != NULL) {
                count++;
                current = current->next;
            }

            // If the count is less than 3, print the room number
            if (count < 3) {
                printf("%d  ", room);
                any_incomplete = true;
            } else {
                printf("     "); // Print spaces for rooms with 3 or more nodes
            }

            // Add a newline after every 10 rooms
            if ((room % 10) == 0) {
                printf("\n");
            }
        }
    }

    // If no incomplete rooms were found, print a message
    if (!any_incomplete) {
        printf("\nNo rooms with less than 3 students.\n");
    }
}


int all_rooms_full(hash_table *ht) {
    // Room ranges for each floor
    int floors[][2] = {
        {101, 120}, // Floor 1: Room 101 - 120
        {201, 220}, // Floor 2: Room 201 - 220
        {301, 320}  // Floor 3: Room 301 - 320
    };

    // Traverse each floor
    for (int f = 0; f < 3; f++) {
        int start = floors[f][0]; // Start room number
        int end = floors[f][1];   // End room number

        // Loop through the rooms in the floor
        for (int room = start; room <= end; room++) {
            int index = hash_function(room, ht->size);
            student_node *current = ht->table[index];
            int count = 0;

            // Count the number of nodes (students) at this index
            while (current != NULL) {
                count++;
                current = current->next;
            }

            // If any room has fewer than 3 nodes, return 0
            if (count < 3) {
                return 0;
            }
        }
    }

    // If all rooms have exactly 3 nodes, return 1
    return 1;
}
