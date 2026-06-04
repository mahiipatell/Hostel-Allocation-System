#include <stdio.h>
#include "../include/setting_up.h"
#include "../include/hash.h"
#include "../include/stack.h"
#include "../include/queue.h"

#define OUTPUT_FILE "sorted_students.csv"
#define CSV_FILE "data/student_data.csv"

// link logistic.c data_to_sorted_csv.c sorted_csv_to_dll.c linked_list.c hash.c stack.c queue.c while executing
int main()
{

    // inputting logistical information from admin
    int l[11];
    input_logistics(l);

    // creating sorted csv file from data
    student_node students[MAX_STUDENTS];           // Array of student nodes
    int count = read_students(CSV_FILE, students); // Read students from CSV file
    calculate_weighted_cgpa(students, count);      // Calculate weighted CGPA
    qsort(students, count, sizeof(student_node), compare_students);
    write_students(OUTPUT_FILE, students, count); // Write sorted students to new CSV file
    printf("Sorted student data has been written to %s\n", OUTPUT_FILE);

    // Create a doubly linked list from the sorted student data
    const char *filename = "sorted_students.csv";          // CSV file name
    student_node_dll list = create_dll_from_csv(filename); // Create DLL from sorted CSV
    // print_list(&list);  // Print the doubly linked list*/

    // hash function initialize
    hash_table *fixed_rooms = create_hash_table(l[0] * l[1], l[0], l[1]);
    hash_table *hold_rooms = create_hash_table(l[0] * l[1], l[0], l[1]);

    // all 6 stacks initialize as counters
    stack mechanical_general;
    stack mechanical_reserved;
    stack computer_general;
    stack computer_reserved;
    stack electrical_general;
    stack electrical_reserved;

    init_stack(&mechanical_general, l[5] - l[8]);
    init_stack(&mechanical_reserved, l[8]);
    init_stack(&computer_general, l[4] - l[7]);
    init_stack(&computer_reserved, l[7]);
    init_stack(&electrical_general, l[6] - l[9]);
    init_stack(&electrical_reserved, l[9]);

    // initialize waiting list (queue)
    queue waiting_list;
    init_queue(&waiting_list);

    student_node *temp = list.head;

    while (temp != NULL)
    {
        printf("------------------------------------------------------\n");
        printf("------------------------------------------------------\n");

        int skip_student = 0;
        // PRINT STUDENT NAME IN ORDER
        printf("Student Name: %s\n", temp->name); // Print the student's name first

        // ASK IF THEY WANT HOSTEL IF NOT SKIP TO NEXT STUDENT
        char hostel_choice;
        printf("Do you want hostel? (y/n): ");
        scanf(" %c", &hostel_choice); // Space before %c to consume any newline characters

        // If the user does not want to check, move to the next student
        if (hostel_choice == 'n' || hostel_choice == 'N')
        {
            // printf("Skipping to the next student.\n");
            temp = temp->next; // Move to the next node (next student)
            continue;          // Skip the remaining logic for this student
        }

        // CONFIRM MIS GIVE THREE CHANCES TO CORRECT MISTAKE
        int chances = 3;  // Initialize chances for each student
         unsigned long long entered_mis;
         // Ask the user for MIS and check with three chances
         while (chances > 0) {
             printf("Please enter your MIS number\n");
             scanf("%I64u", &entered_mis);

             if (entered_mis == temp->mis || entered_mis == 1234) {
                if(entered_mis == 1234){
                    printf("Admin Override entered\n");
                }else{
                    printf("Correct MIS\n");
                }
                 break;  // If MIS is correct, move to the next student
             } else {
                 chances--;
                 if (chances > 0) {
                     printf("Incorrect MIS. You have %d attempt(s) remaining.\n", chances);
                 } else {
                     printf("Sorry, you've used all your attempts for this student. Moving to the next student.\n");
                     skip_student = 1;
                 }
             }
         }

         if(skip_student == 1){
         temp = temp->next;
         continue;
        }

        // Check if the student has available space in the open category for their branch. 
        // If not, check if the student's category is reserved. 
        // If the category is reserved, verify if there is space available in the reserved category. 
        // If space is available, allot the seat, otherwise, move the student to the waiting list.
        if (strcmp(temp->branch, "mechanical") == 0)
        {
            if (is_stack_full(&mechanical_general))
            {
                if (strcmp(temp->category, "reserved") == 0)
                {
                    if (is_stack_full(&mechanical_reserved))
                    {
                        enqueue(&waiting_list, temp);
                        printf("No room available in Mechanical (Open or Reserved), added to the waiting list.\n");
                        temp = temp->next;
                        continue;
                    }
                    else
                    {
                        push(&mechanical_reserved, *temp);
                        printf("Room allocated in Mechanical (Reserved) as Open is full.\n");
                    }
                }
                else
                {
                    enqueue(&waiting_list, temp);
                    printf("No room available in Mechanical (Open), added to the waiting list.\n");
                    temp = temp->next;
                    continue;
                }
            }
            else
            {
                push(&mechanical_general, *temp);
                printf("Room allocated in Mechanical (Open).\n");
            }
        }
        else if (strcmp(temp->branch, "computer") == 0)
        {
            if (is_stack_full(&computer_general))
            {
                if (strcmp(temp->category, "reserved") == 0)
                {
                    if (is_stack_full(&computer_reserved))
                    {
                        enqueue(&waiting_list, temp);
                        printf("No room available in Computer (Open or Reserved), added to the waiting list.\n");
                        temp = temp->next;
                        continue;
                    }
                    else
                    {
                        push(&computer_reserved, *temp);
                        printf("Room allocated in Computer (Reserved) as Open is full.\n");
                    }
                }
                else
                {
                    enqueue(&waiting_list, temp);
                    printf("No room available in Computer (Open), added to the waiting list.\n");
                    temp = temp->next;
                    continue;
                }
            }
            else
            {
                push(&computer_general, *temp);
                printf("Room allocated in Computer (Open).\n");
            }
        }
        else if (strcmp(temp->branch, "electrical") == 0)
        {
            if (is_stack_full(&electrical_general))
            {
                if (strcmp(temp->category, "reserved") == 0)
                {
                    if (is_stack_full(&electrical_reserved))
                    {
                        enqueue(&waiting_list, temp);
                        printf("No room available in Electrical (Open or Reserved), added to the waiting list.\n");
                        temp = temp->next;
                        continue;
                    }
                    else
                    {
                        push(&electrical_reserved, *temp);
                        printf("Room allocated in Electrical (Reserved) as Open is full.\n");
                    }
                }
                else
                {
                    enqueue(&waiting_list, temp);
                    printf("No room available in Electrical (Open), added to the waiting list.\n");
                    temp = temp->next;
                    continue;
                }
            }
            else
            {
                push(&electrical_general, *temp);
                printf("Room allocated in Electrical (Open).\n");
            }
        }

        int room_number = 0;

        // IF PUT ON HOLD BY SOMEBODY ELSE
        bool on_hold = search_by_merit_no(fixed_rooms, hold_rooms, temp->merit_no);
        if (on_hold)
        {
            printf("These rooms have asked for you to be their roommate\n");
            printf("Enter desired room out of the following, if none enter 0\n");
            scanf("%d", &room_number);

            if (room_number != 0)
            {
                int index = hash_function(room_number, l[0], l[1]);
                append_to_hash(fixed_rooms, index, info_from_merit_no(&list, temp->merit_no));
                temp->room_no = room_number;
                printf("Your room has been allotted\n");
            }
        }
        
        // IF NOT PUT ON HOLD OR REJECTED ALLOCATED ROOM
        if (!on_hold || room_number == 0)
        {
            // display empty
            printf("Enter desired room out of the following\n");
            print_empty_rooms(fixed_rooms);
            printf("\n");
            scanf("%d", &room_number);
            // table size is total rooms ie total floors into total rooms
            int index =  hash_function(room_number, l[0], l[1]);
            // add name to fixed room list
            int check = append_to_hash(fixed_rooms, index, info_from_merit_no(&list, temp->merit_no));
            if(check == -1) {
                temp = temp->next;
                continue;
            }
            temp->room_no = room_number;

            int roommate_merit = -1;
            printf("Your room has been allotted\n");

            // ASK FOR OTHER TWO ROOMMATES
            printf("Roommate merit no should be lower than yours: ");
            printf("Enter merit no of roommate 1: ");
            scanf("%d", &roommate_merit);
            append_to_hash(hold_rooms, index, info_from_merit_no(&list, roommate_merit));
            printf("Enter merit no of roommate 2: ");
            scanf("%d", &roommate_merit);
            append_to_hash(hold_rooms, index, info_from_merit_no(&list, roommate_merit));
        }

        //remove them from hold hash table
        remove_from_hash(hold_rooms, temp->merit_no);

        // CHECK PRINT FUNCTION ADDED
       // print_hash(fixed_rooms);
       // printf("\n");
        //print_hash(hold_rooms);
        /*
        printf("mech_open\n");
        print_stack(mechanical_general);
        printf("mech_res\n");
        print_stack(mechanical_reserved); // Print Mechanical (Reserved) stack
        printf("comp_open\n");
        print_stack(computer_general); // Print Computer (General) stack
        printf("comp_res\n");
        print_stack(computer_reserved);
        printf("electric_open\n");
        print_stack(electrical_general); // Print Electrical (General) stack
        printf("electric_res\n");
        print_stack(electrical_reserved); // Print Electrical (Reserved) stack

        print_queue(&waiting_list);*/

        if (temp->room_no == -1)
        {
            //enqueue(&waiting_list, temp);
            //IF SOMEONE HAS A VALID SPACE BUT NO EMPTY ROOMS THEY SHOULD BE ABLE TO TAKE A HELD SPACE OF SOMEBODY
            // check which fixed room is not filled by three people show those rooms then append to fixed on that index go to hold and remove last node
            printf("Enter desired room out of the following\n");
            print_incomplete_rooms(fixed_rooms);
            printf("\n");
            scanf("%d", &room_number);
            // table size is total rooms ie total floors into total rooms
            int index = hash_function(room_number, l[0], l[1]);
            // add name to fixed room list
            append_to_hash(fixed_rooms, index, info_from_merit_no(&list, temp->merit_no));
            temp->room_no = room_number;
             // Properly remove the last held roommate at this index
            if (hold_rooms->table[index] != NULL) {
                student_node *last = hold_rooms->table[index];
                while (last->next != NULL) last = last->next;
                remove_from_hash(hold_rooms, last->merit_no);
            }
        }

        else
        {
            // Open the file for appending
            FILE *file = fopen("allocated_rooms.csv", "a");
            if (file == NULL)
            {
                perror("Failed to open file");
                exit(EXIT_FAILURE);
            }

            // Write current node's information to the file
            fprintf(file, "MIS: %I64u, Name: %s, Branch: %s, Category: %s, Room Number: %d\n",
                    temp->mis, temp->name, temp->branch, temp->category, temp->room_no);

            // Close the file
            fclose(file);
        }

        temp = temp->next; // Move to the next node (next student)
    }







   //NOW TO DO WAITING LIST
   printf("Waiting List Allotment\n");
     queue_node *temps = waiting_list.front;
    while (temps != NULL){
        student_node* tempw = temps->student;
        if(all_rooms_full(fixed_rooms)){
            printf("No more space available");
            break;
        }
        printf("------------------------------------------------------\n");
        printf("------------------------------------------------------\n");

        int skip_student = 0;
        // PRINT STUDENT NAME IN ORDER
        printf("Student Name: %s\n", tempw->name); // Print the student's name first

         // CONFIRM MIS GIVE THREE CHANCES TO CORRECT MISTAKE
        int chances = 3;  // Initialize chances for each student
         unsigned long long entered_mis;
         // Ask the user for MIS and check with three chances
         while (chances > 0) {
             printf("Please enter your MIS number\n");
             scanf("%I64u", &entered_mis);

             if (entered_mis == tempw->mis || entered_mis == 1234) {
                if(entered_mis == 1234){
                    printf("Admin Override entered\n");
                }else{
                    printf("Correct MIS\n");
                }
                 break;  // If MIS is correct, move to the next student
             } else {
                 chances--;
                 if (chances > 0) {
                     printf("Incorrect MIS. You have %d attempt(s) remaining.\n", chances);
                 } else {
                     printf("Sorry, you've used all your attempts for this student. Moving to the next student.\n");
                     skip_student = 1;
                 }
             }
         }

         if(skip_student == 1){
         temps = temps->next;
         continue;
        }

       // Re-check branch quota before allocating from waitlist
        int quota_available = 0;
        if (strcmp(tempw->branch, "mechanical") == 0) {
            if (!is_stack_full(&mechanical_general)) {
                push(&mechanical_general, *tempw);
                quota_available = 1;
            } else if (strcmp(tempw->category, "reserved") == 0 && !is_stack_full(&mechanical_reserved)) {
                push(&mechanical_reserved, *tempw);
                quota_available = 1;
            }
        } else if (strcmp(tempw->branch, "computer") == 0) {
            if (!is_stack_full(&computer_general)) {
                push(&computer_general, *tempw);
                quota_available = 1;
            } else if (strcmp(tempw->category, "reserved") == 0 && !is_stack_full(&computer_reserved)) {
                push(&computer_reserved, *tempw);
                quota_available = 1;
            }
        } else if (strcmp(tempw->branch, "electrical") == 0) {
            if (!is_stack_full(&electrical_general)) {
                push(&electrical_general, *tempw);
                quota_available = 1;
            } else if (strcmp(tempw->category, "reserved") == 0 && !is_stack_full(&electrical_reserved)) {
                push(&electrical_reserved, *tempw);
                quota_available = 1;
            }
        }

        if (!quota_available) {
            printf("No quota available for %s. Skipping.\n", tempw->name);
            temps = temps->next;
            continue;
        }

        int room_number;
        printf("Enter desired room out of the following\n");
        print_incomplete_rooms(fixed_rooms);
        printf("\n");
        scanf("%d", &room_number);
        // table size is total rooms ie total floors into total rooms
        int index = hash_function(room_number, l[0], l[1]);
        // add name to fixed room list
        int check = append_to_hash(fixed_rooms, index, info_from_merit_no(&list, tempw->merit_no));
        if (check == -1) {
            temps = temps->next;
            continue;
        }
        tempw->room_no = room_number;
    
    // Open the file for appending
            FILE *file = fopen("allocated_rooms.csv", "a");
            if (file == NULL)
            {
                perror("Failed to open file");
                exit(EXIT_FAILURE);
            }

            // Write current node's information to the file
            fprintf(file, "MIS: %I64u, Name: %s, Branch: %s, Category: %s, Room Number: %d\n",
                    tempw->mis, tempw->name, tempw->branch, tempw->category, tempw->room_no);

            // Close the file
            fclose(file);
        

        temps = temps->next; // Move to the next node (next student)
    }

    return 0;
}
