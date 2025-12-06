#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUDENT_FILE    "students.txt"
#define CREDENTIAL_FILE "credentials.txt"

struct Student {
    int roll;
    char name[50];
    float marks;
};

char currentRole[10];
char currentUser[50];

/* Function prototypes */
int  loginSystem(void);
void mainMenu(void);

void adminMenu(void);
void staffMenu(void);
void guestMenu(void);
void userMenu(void);

void addStudent(void);
void displayStudents(void);
void searchStudent(void);
void updateStudent(void);
void deleteStudent(void);

/*-------------------------------------- MAIN -----------------------------------*/
int main() {
    int attempts = 0;
    int success  = 0;

    while (attempts < 3) {
        if (loginSystem()) {
            success = 1;
            break;
        } else {
            attempts++;
            if (attempts < 3) {
                printf("\nInvalid username or password. Try again.\n\n");
            }
        }
    }

    if (!success) {
        printf("\nLogin Failed. Exiting...\n");
        return 0;
    }

    mainMenu();
    return 0;
}

/*----------------------------------- LOGIN ----------------------------------*/
int loginSystem() {
    char username[50], password[50];
    char fileUser[50], filePass[50], fileRole[10];

    printf("===== Login Screen =====\n");
    printf("Username: ");
    scanf("%49s", username);
    printf("Password: ");
    scanf("%49s", password);

    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if (!fp) {
        printf("Error: %s not found!\n", CREDENTIAL_FILE);
        return 0;
    }

    while (fscanf(fp, "%49s %49s %9s", fileUser, filePass, fileRole) == 3) {
        if (strcmp(username, fileUser) == 0 &&
            strcmp(password, filePass) == 0) {

            strcpy(currentRole, fileRole);
            strcpy(currentUser, fileUser);
            fclose(fp);

            printf("\nLogin successful! Welcome, %s (%s)\n\n",
                   currentUser, currentRole);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

/*-------------------------------------- MAIN MENU -----------------------------------*/
void mainMenu() {
    if (strcmp(currentRole, "ADMIN") == 0)
        adminMenu();
    else if (strcmp(currentRole, "STAFF") == 0)
        staffMenu();
    else if (strcmp(currentRole, "GUEST") == 0)
        guestMenu();
    else
        userMenu();   /* USER role */
}

/*--------------------------- ADMIN MENU ---------------------------*/
void adminMenu() {
    int choice;
    do {
        printf("====== ADMIN MENU ======\n");
        printf("1. Add New Student\n");
        printf("2. Display All Records\n");
        printf("3. Search Record\n");
        printf("4. Update Student Details\n");
        printf("5. Delete Student Record\n");
        printf("6. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent();       break;
            case 2: displayStudents();  break;
            case 3: searchStudent();    break;
            case 4: updateStudent();    break;
            case 5: deleteStudent();    break;
            case 6: printf("Logging out...\n"); return;
            default: printf("Invalid choice!\n");
        }
        printf("\n");
    } while (1);
}

/*--------------------------- STAFF MENU ---------------------------*/
/* STAFF: display, search, update */
void staffMenu() {
    int choice;
    do {
        printf("====== STAFF MENU ======\n");
        printf("1. Display Students\n");
        printf("2. Search Student\n");
        printf("3. Update Student\n");
        printf("4. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: displayStudents();  break;
            case 2: searchStudent();    break;
            case 3: updateStudent();    break;
            case 4: printf("Logging out...\n"); return;
            default: printf("Invalid choice!\n");
        }
        printf("\n");
    } while (1);
}

/*--------------------------- GUEST MENU ---------------------------*/
/* GUEST: display, search */
void guestMenu() {
    int choice;
    do {
        printf("====== GUEST MENU ======\n");
        printf("1. Display Students\n");
        printf("2. Search Student\n");
        printf("3. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: displayStudents();  break;
            case 2: searchStudent();    break;
            case 3: printf("Logging out...\n"); return;
            default: printf("Invalid choice!\n");
        }
        printf("\n");
    } while (1);
}

/*--------------------------- USER MENU ---------------------------*/
/* USER: display only */
void userMenu() {
    int choice;
    do {
        printf("====== USER MENU ======\n");
        printf("1. Display Students\n");
        printf("2. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: displayStudents();  break;
            case 2: printf("Logging out...\n"); return;
            default: printf("Invalid choice!\n");
        }
        printf("\n");
    } while (1);
}

/*------------------------- STUDENT FUNCTIONS -------------------------*/
 void addStudent() {
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "a");
    if (!fp) {
        printf("Error opening %s\n", STUDENT_FILE);
        return;
    }

    printf("Enter roll number: ");
    scanf("%d", &s.roll);

    printf("Enter name (single word): ");
    scanf("%49s", s.name);

    printf("Enter marks: ");
    scanf("%f", &s.marks);

    fprintf(fp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    fclose(fp);

    printf("Student added successfully.\n");
}


void displayStudents() {
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("No records found.\n");
        return;
    }

    printf("\nRoll\tName\tMarks\n");
    printf("---------------------------\n");
    while (fscanf(fp, "%d %49s %f", &s.roll, s.name, &s.marks) == 3) {
        printf("%d\t%s\t%.2f\n", s.roll, s.name, s.marks);
    }
    fclose(fp);
}

/* Search by roll OR name */
void searchStudent() {
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "r");
    int choice;
    int roll, found = 0;
    char searchName[50];

    if (!fp) {
        printf("No records found.\n");
        return;
    }

    printf("Search Student\n");
    printf("1. Search by Roll Number\n");
    printf("2. Search by Name\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Enter roll number to search: ");
        scanf("%d", &roll);

        while (fscanf(fp, "%d %49s %f", &s.roll, s.name, &s.marks) == 3) {
            if (s.roll == roll) {
                printf("\nRecord found:\n");
                printf("Roll : %d\nName : %s\nMarks: %.2f\n",
                       s.roll, s.name, s.marks);
                found = 1;
                break;
            }
        }
    } else if (choice == 2) {
        printf("Enter name to search: ");
        scanf("%49s", searchName);

        while (fscanf(fp, "%d %49s %f", &s.roll, s.name, &s.marks) == 3) {
            if (strcmp(s.name, searchName) == 0) {
                printf("\nRecord found:\n");
                printf("Roll : %d\nName : %s\nMarks: %.2f\n",
                       s.roll, s.name, s.marks);
                found = 1;
                break;
            }
        }
    } else {
        printf("Invalid choice.\n");
        fclose(fp);
        return;
    }

    if (!found)
        printf("\nRecord not found.\n");

    fclose(fp);
}

void updateStudent() {
    struct Student s[100];
    int n = 0;
    int roll, i, found = 0;
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("No records found.\n");
        return;
    }

    while (fscanf(fp, "%d %49s %f", &s[n].roll, s[n].name, &s[n].marks) == 3) {
        n++;
    }
    fclose(fp);

    printf("Enter roll number to update: ");
    scanf("%d", &roll);

    for (i = 0; i < n; i++) {
        if (s[i].roll == roll) {
            printf("Enter new name: ");
            scanf("%49s", s[i].name);
            printf("Enter new marks: ");
            scanf("%f", &s[i].marks);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Record not found.\n");
        return;
    }

    fp = fopen(STUDENT_FILE, "w");
    if (!fp) {
        printf("Error opening %s\n", STUDENT_FILE);
        return;
    }

    for (i = 0; i < n; i++) {
        fprintf(fp, "%d %s %.2f\n", s[i].roll, s[i].name, s[i].marks);
    }
    fclose(fp);

    printf("Record updated successfully.\n");
}

void deleteStudent() {
    struct Student s[100];
    int n = 0;
    int roll, i, j, found = 0;
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("No records found.\n");
        return;
    }

    while (fscanf(fp, "%d %49s %f", &s[n].roll, s[n].name, &s[n].marks) == 3) {
        n++;
    }
    fclose(fp);

    printf("Enter roll number to delete: ");
    scanf("%d", &roll);

    for (i = 0; i < n; i++) {
        if (s[i].roll == roll) {
            found = 1;
            for (j = i; j < n - 1; j++) {
                s[j] = s[j + 1];
            }
            n--;
            break;
        }
    }

    if (!found) {
        printf("Record not found.\n");
        return;
    }

    fp = fopen(STUDENT_FILE, "w");
    if (!fp) {
        printf("Error opening %s\n", STUDENT_FILE);
        return;
    }

    for (i = 0; i < n; i++) {
        fprintf(fp, "%d %s %.2f\n", s[i].roll, s[i].name, s[i].marks);
    }
    fclose(fp);

    printf("Record deleted successfully.\n");
}

