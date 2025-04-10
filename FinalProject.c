#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

typedef struct {
    int id;
    char name[50];
    int age;
    char gender[10];
    char course[30];
    float marks;
    int paid; // 1 for paid, 0 for not paid
} Student;

Student students[MAX];
int count = 0;
const char *filename = "students.txt";

void saveToFile() {
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        printf("Error saving to file.\n");
        return;
    }
    fwrite(&count, sizeof(int), 1, fp);
    fwrite(students, sizeof(Student), count, fp);
    fclose(fp);
}

void loadFromFile() {
    FILE *fp = fopen(filename, "rb");
    if (fp != NULL) {
        fread(&count, sizeof(int), 1, fp);
        fread(students, sizeof(Student), count, fp);
        fclose(fp);
    }
}

void trimNewline(char *str) {
    str[strcspn(str, "\n")] = '\0';
}

void addStudent() {
    if (count >= MAX) {
        printf("Maximum student limit reached!\n");
        return;
    }
    Student s;
    printf("Enter ID: "); scanf("%d", &s.id);
    getchar(); // consume newline
    printf("Enter Full Name: "); fgets(s.name, sizeof(s.name), stdin); trimNewline(s.name);
    printf("Enter Age: "); scanf("%d", &s.age);
    printf("Enter Gender: "); scanf("%s", s.gender);
    printf("Enter Course: "); scanf("%s", s.course);
    printf("Enter Marks: "); scanf("%f", &s.marks);
    printf("Payment Done? (1 for Yes, 0 for No): "); scanf("%d", &s.paid);

    students[count++] = s;
    saveToFile();
    printf("Student added successfully!\n");
}

void viewStudents() {
    printf("\n--- All Students ---\n");
    for (int i = 0; i < count; i++) {
        printf("ID: %d | Name: %s | Age: %d | Gender: %s | Course: %s | Marks: %.2f | Paid: %s\n",
            students[i].id, students[i].name, students[i].age, students[i].gender,
            students[i].course, students[i].marks, students[i].paid ? "Yes" : "No");
    }
}

void searchStudent() {
    int choice, idFound = 0;
    char name[50];
    printf("Search by:\n1. ID\n2. Name:\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    getchar(); // consume newline
    if (choice == 1) {
        int id;
        printf("Enter ID: "); scanf("%d", &id);
        for (int i = 0; i < count; i++) {
            if (students[i].id == id) {
                printf("Found: %s | Course: %s | Marks: %.2f\n", students[i].name, students[i].course, students[i].marks);
                idFound = 1;
                break;
            }
        }
    } else {
        printf("Enter Name: "); fgets(name, sizeof(name), stdin); trimNewline(name);
        for (int i = 0; i < count; i++) {
            if (strcmp(students[i].name, name) == 0) {
                printf("Found: ID: %d | Course: %s | Marks: %.2f\n", students[i].id, students[i].course, students[i].marks);
                idFound = 1;
            }
        }
    }
    if (!idFound) printf("Student not found!\n");
}

void updateStudent() {
    int id, found = 0;
    printf("Enter ID to update: "); scanf("%d", &id);
    getchar(); // consume newline
    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            found = 1;
            int updateChoice;
            do {
                printf("\n--- Update Menu ---\n");
                printf("1. ID\n2. Name\n3. Age\n4. Gender\n5. Course\n6. Marks\n7. Payment\n8. Exit Update Menu\n");
                printf("Enter what you want to update: ");
                scanf("%d", &updateChoice);
                getchar(); // consume newline

                switch (updateChoice) {
                    case 1:
                        printf("Enter new ID: ");
                        scanf("%d", &students[i].id);
                        break;
                    case 2:
                        printf("Enter new Full Name: ");
                        fgets(students[i].name, sizeof(students[i].name), stdin); trimNewline(students[i].name);
                        break;
                    case 3:
                        printf("Enter new Age: ");
                        scanf("%d", &students[i].age);
                        break;
                    case 4:
                        printf("Enter new Gender: ");
                        scanf("%s", students[i].gender);
                        break;
                    case 5:
                        printf("Enter new Course: ");
                        scanf("%s", students[i].course);
                        break;
                    case 6:
                        printf("Enter new Marks: ");
                        scanf("%f", &students[i].marks);
                        break;
                    case 7:
                        printf("Enter Payment Status (1 for Paid, 0 for Not Paid): ");
                        scanf("%d", &students[i].paid);
                        break;
                    case 8:
                        printf("Exiting update menu...\n");
                        break;
                    default:
                        printf("Invalid choice! Try again.\n");
                }
            } while (updateChoice != 8);

            saveToFile();
            printf("Student updated successfully!\n");
            break;
        }
    }
    if (!found) {
        printf("Student not found!\n");
    }
}

void deleteStudent() {
    int id;
    printf("Enter ID to delete: "); scanf("%d", &id);
    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            for (int j = i; j < count - 1; j++) {
                students[j] = students[j+1];
            }
            count--;
            saveToFile();
            printf("Student deleted successfully!\n");
            return;
        }
    }
    printf("Student not found!\n");
}

void sortStudents() {
    int choice;
    printf("Sort by:\n1. ID\n2. Name\n3. Age\n4. Marks\n5. Payment\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            int cond = 0;
            switch (choice) {
                case 1: cond = students[i].id > students[j].id; break;
                case 2: cond = strcmp(students[i].name, students[j].name) > 0; break;
                case 3: cond = students[i].age > students[j].age; break;
                case 4: cond = students[i].marks < students[j].marks; break;
                case 5: cond = students[i].paid < students[j].paid; break;
                default: printf("Invalid choice!\n"); return;
            }
            if (cond) {
                Student temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }
    }
    viewStudents();
}

void calculateResults() {
    for (int i = 0; i < count; i++) {
        printf("%s (ID: %d) - %.2f - %s\n", students[i].name, students[i].id, students[i].marks,
            students[i].marks >= 40.0 ? "Pass" : "Fail");
    }
}

void checkPayments() {
    for (int i = 0; i < count; i++) {
        printf("%s (ID: %d) - Payment: %s\n", students[i].name, students[i].id,
            students[i].paid ? "Paid" : "Not Paid");
    }
}

void generateBill() {
    int id, found = 0;
    printf("Enter Student ID to generate bill: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            found = 1;
            if (students[i].paid) {
                printf("\n===== STUDENT BILL =====\n");
                printf("Name     : %s\n", students[i].name);
                printf("ID       : %d\n", students[i].id);
                printf("Course   : %s\n", students[i].course);
                printf("Status   : Paid\n");
                printf("Thank you for your payment!\n");
                printf("=========================\n");
            } else {
                printf("Payment not done. Cannot generate bill.\n");
            }
            break;
        }
    }

    if (!found) {
        printf("Student with ID %d not found!\n", id);
    }
}

int main() {
    loadFromFile();
    int choice;
    printf("\n==========================================\n");
    printf("     Student Record Management System\n");
    printf("==========================================\n");

    do {
        printf("\n--- Main Menu ---\n");
        printf("1. Add Student\n2. View All Students\n3. Search Student\n4. Update Student\n5. Delete Student\n");
        printf("6. Sort Records\n7. Calculate Results\n8. Check Payments\n9. Generate Bill\n10. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: sortStudents(); break;
            case 7: calculateResults(); break;
            case 8: checkPayments(); break;
            case 9: generateBill(); break;
            case 10: saveToFile(); printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 10);

    return 0;
}
