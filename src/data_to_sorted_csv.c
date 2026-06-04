#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/setting_up.h"

#define MAX_STUDENTS 1000
#define OUTPUT_FILE "sorted_students.csv"

// Function to calculate mean and standard deviation
void calculate_statistics(student_node *students, int count, const char *branch, float *mean, float *stddev) {
    float sum = 0.0;
    int i, valid_count = 0;

    // Calculate mean
    for (i = 0; i < count; i++) {
        if (strcmp(students[i].branch, branch) == 0) {
            sum += students[i].cgpa;
            valid_count++;
        }
    }

    if (valid_count > 0) {
        *mean = sum / valid_count;

        // Calculate standard deviation
        sum = 0.0;
        for (i = 0; i < count; i++) {
            if (strcmp(students[i].branch, branch) == 0) {
                sum += pow(students[i].cgpa - *mean, 2);
            }
        }
        *stddev = sqrt(sum / valid_count);
    } else {
        *mean = 0.0;
        *stddev = 0.0;
    }
}

// Function to read the CSV file and populate the student array
int read_students(const char *filename, student_node *students) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open file");
        return 0;
    }

    int count = 0;
    while (count < MAX_STUDENTS && fscanf(file, "%*[^,],%99[^,],%I64u,%14[^,],%14[^,],%f\n",
        students[count].name, &students[count].mis,
        students[count].branch, students[count].category, &students[count].cgpa) == 5) {
        
        students[count].room_no = -1;  // Initialize room_no to -1
        students[count].weighted_cgpa = 0.0;  // Initialize weighted CGPA
        count++;
    }

    fclose(file);
    return count;
}

// Function to calculate weighted CGPA for each student
void calculate_weighted_cgpa(student_node *students, int count) {
    float mean, stddev;

    // Calculate weighted CGPA for each branch
    for (int i = 0; i < count; i++) {
        calculate_statistics(students, count, students[i].branch, &mean, &stddev);
        if (stddev != 0) {  // Avoid division by zero
            students[i].weighted_cgpa = (students[i].cgpa - mean) / stddev;
        }
    }
}

// Comparator function for sorting students
int compare_students(const void *a, const void *b) {
    const student_node *studentA = (const student_node *)a;
    const student_node *studentB = (const student_node *)b;

    // First compare by weighted CGPA (descending order)
    if (studentA->weighted_cgpa < studentB->weighted_cgpa) {
        return 1; // studentA comes after studentB
    } else if (studentA->weighted_cgpa > studentB->weighted_cgpa) {
        return -1; // studentA comes before studentB
    }

    // If weighted CGPA is the same, compare alphabetically by name
    return strcmp(studentA->name, studentB->name);
}

// Function to write the sorted student array to a new CSV file
void write_students(const char *filename, student_node *students, int count) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Unable to open file for writing");
        return;
    }

    for (int i = 0; i < count; i++) {
        students[i].merit_no = i + 1;  // Assign chronological merit number
        fprintf(file, "%u,%s,%I64u,%s,%s,%f,%d\n",
            students[i].merit_no, students[i].name, students[i].mis,
            students[i].branch, students[i].category, students[i].weighted_cgpa, students[i].room_no);
    }

    fclose(file);
}


