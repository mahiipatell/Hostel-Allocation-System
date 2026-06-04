#include <stdio.h>

void input_logistics(int logistics[]){
    printf("ADMIN ONLY\n");
    printf("Enter the number of floors: ");     //3
    scanf("%d", &logistics[0]);

    printf("Enter the number of rooms on a floor: ");     //20
    scanf("%d", &logistics[1]);

    printf("Enter the total number of students: ");     //250
    scanf("%d", &logistics[2]);

    printf("Enter the total number of branches: ");
    scanf("%d", &logistics[3]);

    printf("Enter the number of seats for Computer: ");
    scanf("%d", &logistics[4]);

    printf("Enter the number of seats for Mechanical: ");
    scanf("%d", &logistics[5]);

    printf("Enter the number of seats for Electrical: ");
    scanf("%d", &logistics[6]);

    printf("Enter the reserved seats for Computer: ");
    scanf("%d", &logistics[7]);

    printf("Enter the reserved seats for Mechanical: ");
    scanf("%d", &logistics[8]);

    printf("Enter the reserved seats for Electrical: ");
    scanf("%d", &logistics[9]);

}

