#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include "setting_up.h"

// Function declarations
void init_dll(student_node_dll *dlls); 
void insert_end_dll(student_node_dll *dlls, student_node sn);
student_node *remove_student_node(student_node_dll *dlls, student_node *sn);
student_node *search_student(student_node_dll *dlls, unsigned long long int mis);
student_node *info_from_merit_no(student_node_dll *dlls, unsigned int merit_no);


#endif // LINKED_LIST_H
