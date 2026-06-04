#include "../include/setting_up.h"
#include "../include/hash.h"
#include "../include/linked_list.h"

// Function to create and initialize the hash table, size is total number of rooms 
hash_table* create_hash_table(int size, int floors, int rooms_per_floor) {
    // Allocate memory for the hash table structure
    hash_table* ht = (hash_table*)malloc(sizeof(hash_table));
    if (ht == NULL) {
        printf("Memory allocation failed for hash table.\n");
        return NULL;    // Return NULL if allocation fails
    }
    ht->size = size;    // Set the size of the hash table
    ht->floors = floors;
    ht->rooms_per_floor = rooms_per_floor;

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


// Hash function — works for any number of floors and rooms
int hash_function(int room_number, int floors, int rooms_per_floor) {
    int floor_no = room_number / 100;         // e.g. 301 → floor 3
    int room_no  = room_number % 100;         // e.g. 301 → room 1

    if (floor_no < 1 || floor_no > floors)
        return -1;
    if (room_no < 1 || room_no > rooms_per_floor)
        return -1;

    return (floor_no - 1) * rooms_per_floor + (room_no - 1);
}

// Reverse hash — converts index back to room number
int reverse_hash_function(int index, int rooms_per_floor) {
    if (index < 0) return -1;
    int floor_no = index / rooms_per_floor + 1;
    int room_no  = index % rooms_per_floor + 1;
    return floor_no * 100 + room_no;
}

// Function to print room numbers and student names
void print_hash(hash_table* ht) {
    for (int i = 0; i < ht->size; i++) {
        if (ht->table[i] != NULL) {  // Check if the bucket is not empty
            student_node* current = ht->table[i];  // Start at the head of the linked list
            
            while (current != NULL) {  // Traverse the linked list
                int room_number = reverse_hash_function(i, ht->rooms_per_floor);
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
                int room_no = reverse_hash_function(current_index, ht->rooms_per_floor);
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
    
    // Traverse the entire hash table
    for (int i = 0; i < ht->size; i++) {
        student_node* current = ht->table[i];
        student_node* to_delete = NULL;

        // Traverse the linked list at this index
        while (current != NULL) {
            // Check if the current node has the specified merit number
            if (current->merit_no == merit_no) {
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
    bool any_empty = false;

    for (int f = 1; f <= ht->floors; f++) {
        printf("\nFloor %d:\n", f);
        for (int r = 1; r <= ht->rooms_per_floor; r++) {
            int room = f * 100 + r;
            int index = hash_function(room, ht->floors, ht->rooms_per_floor);
            if (ht->table[index] == NULL) {
                printf("%d  ", room);
                any_empty = true;
            } else {
                printf("     ");
            }
            if (r % 10 == 0) printf("\n");
        }
    }

    if (!any_empty) printf("\nNo empty rooms available.\n");
}

void print_incomplete_rooms(hash_table *ht) {
    bool any_incomplete = false;

    for (int f = 1; f <= ht->floors; f++) {
        printf("\nFloor %d (Rooms with less than 3 students):\n", f);
        for (int r = 1; r <= ht->rooms_per_floor; r++) {
            int room = f * 100 + r;
            int index = hash_function(room, ht->floors, ht->rooms_per_floor);
            student_node *current = ht->table[index];
            int count = 0;
            while (current != NULL) { count++; current = current->next; }

            if (count < 3) {
                printf("%d  ", room);
                any_incomplete = true;
            } else {
                printf("     ");
            }
            if (r % 10 == 0) printf("\n");
        }
    }

    if (!any_incomplete) printf("\nNo rooms with less than 3 students.\n");
}



int all_rooms_full(hash_table *ht) {
    for (int f = 1; f <= ht->floors; f++) {
        for (int r = 1; r <= ht->rooms_per_floor; r++) {
            int room = f * 100 + r;
            int index = hash_function(room, ht->floors, ht->rooms_per_floor);
            student_node *current = ht->table[index];
            int count = 0;
            while (current != NULL) { count++; current = current->next; }
            if (count < 3) return 0;
        }
    }
    return 1;
}


// Save hash table to binary file
void save_hash_table(hash_table *ht, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        printf("Warning: could not save allocations to %s\n", filename);
        return;
    }

    // Write floors and rooms_per_floor first
    fwrite(&ht->floors, sizeof(int), 1, f);
    fwrite(&ht->rooms_per_floor, sizeof(int), 1, f);

    // For each bucket, write how many students are in it, then each student
    for (int i = 0; i < ht->size; i++) {
        int count = 0;
        student_node *current = ht->table[i];
        while (current != NULL) { count++; current = current->next; }

        fwrite(&count, sizeof(int), 1, f);

        current = ht->table[i];
        while (current != NULL) {
            // Write only the data fields, not the pointers
            fwrite(&current->merit_no, sizeof(unsigned int), 1, f);
            fwrite(current->name, sizeof(char), 100, f);
            fwrite(&current->mis, sizeof(unsigned long long int), 1, f);
            fwrite(current->branch, sizeof(char), 15, f);
            fwrite(current->category, sizeof(char), 15, f);
            fwrite(&current->cgpa, sizeof(float), 1, f);
            fwrite(&current->weighted_cgpa, sizeof(float), 1, f);
            fwrite(&current->room_no, sizeof(int), 1, f);
            current = current->next;
        }
    }

    fclose(f);
    printf("Allocations saved to %s\n", filename);
}

// Load hash table from binary file
// Returns 1 if loaded successfully, 0 if file doesn't exist
int load_hash_table(hash_table *ht, const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) return 0;  // File doesn't exist, fresh start

    int saved_floors, saved_rooms_per_floor;
    fread(&saved_floors, sizeof(int), 1, f);
    fread(&saved_rooms_per_floor, sizeof(int), 1, f);

    // Check if saved config matches current config
    if (saved_floors != ht->floors || saved_rooms_per_floor != ht->rooms_per_floor) {
        printf("Warning: saved allocation config doesn't match current setup. Starting fresh.\n");
        fclose(f);
        return 0;
    }

    for (int i = 0; i < ht->size; i++) {
        int count;
        fread(&count, sizeof(int), 1, f);

        for (int j = 0; j < count; j++) {
            student_node *node = (student_node *)malloc(sizeof(student_node));
            fread(&node->merit_no, sizeof(unsigned int), 1, f);
            fread(node->name, sizeof(char), 100, f);
            fread(&node->mis, sizeof(unsigned long long int), 1, f);
            fread(node->branch, sizeof(char), 15, f);
            fread(node->category, sizeof(char), 15, f);
            fread(&node->cgpa, sizeof(float), 1, f);
            fread(&node->weighted_cgpa, sizeof(float), 1, f);
            fread(&node->room_no, sizeof(int), 1, f);
            node->next = NULL;
            node->prev = NULL;
            append_to_hash(ht, i, node);
        }
    }

    fclose(f);
    printf("Previous allocations loaded from %s\n", filename);
    return 1;
}

// Swap all students between two rooms
void swap_rooms(hash_table *ht, int room_a, int room_b) {
    int index_a = hash_function(room_a, ht->floors, ht->rooms_per_floor);
    int index_b = hash_function(room_b, ht->floors, ht->rooms_per_floor);

    if (index_a == -1 || index_b == -1) {
        printf("Invalid room number(s).\n");
        return;
    }

    // Swap the pointers at the two indices
    student_node *temp = ht->table[index_a];
    ht->table[index_a] = ht->table[index_b];
    ht->table[index_b] = temp;

    // Update room_no field for all students in both rooms
    student_node *current = ht->table[index_a];
    while (current != NULL) {
        current->room_no = room_a;
        current = current->next;
    }

    current = ht->table[index_b];
    while (current != NULL) {
        current->room_no = room_b;
        current = current->next;
    }

    printf("Room %d and Room %d have been swapped.\n", room_a, room_b);
}