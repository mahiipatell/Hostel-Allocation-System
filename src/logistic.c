#include <stdio.h>

void input_logistics(int logistics[]) {
    printf("=== ADMIN SETUP ===\n");

    do {
        printf("Enter the number of floors (1-9): ");
        scanf("%d", &logistics[0]);
    } while (logistics[0] < 1 || logistics[0] > 9);

    do {
        printf("Enter the number of rooms on a floor (1-20): ");
        scanf("%d", &logistics[1]);
    } while (logistics[1] < 1 || logistics[1] > 20);

    do {
        printf("Enter the total number of students: ");
        scanf("%d", &logistics[2]);
    } while (logistics[2] < 1);

    do {
        printf("Enter the total number of branches: ");
        scanf("%d", &logistics[3]);
    } while (logistics[3] < 1);

    do {
        printf("Enter the number of seats for Computer: ");
        scanf("%d", &logistics[4]);
    } while (logistics[4] < 0);

    do {
        printf("Enter the number of seats for Mechanical: ");
        scanf("%d", &logistics[5]);
    } while (logistics[5] < 0);

    do {
        printf("Enter the number of seats for Electrical: ");
        scanf("%d", &logistics[6]);
    } while (logistics[6] < 0);

    do {
        printf("Enter the reserved seats for Computer (max %d): ", logistics[4]);
        scanf("%d", &logistics[7]);
    } while (logistics[7] < 0 || logistics[7] > logistics[4]);

    do {
        printf("Enter the reserved seats for Mechanical (max %d): ", logistics[5]);
        scanf("%d", &logistics[8]);
    } while (logistics[8] < 0 || logistics[8] > logistics[5]);

    do {
        printf("Enter the reserved seats for Electrical (max %d): ", logistics[6]);
        scanf("%d", &logistics[9]);
    } while (logistics[9] < 0 || logistics[9] > logistics[6]);

    printf("\nSetup complete. Total rooms: %d\n\n", logistics[0] * logistics[1]);
}