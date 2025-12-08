#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUD_FILE "students.dat"
#define TKT_FILE  "tickets.dat"
#define CSV_FILE  "students.csv"

typedef struct {
    char id[20];
    char name[50];
    char branch[20];
    char section[10];
    float cgpa;
    char phone[15];
    char password[20];
} Student;

typedef struct {
    int ticketId;
    char studentId[20];
    char message[200];
    char status[10];
} Ticket;

void readLine(char *buf, size_t size) {
    if (fgets(buf, (int)size, stdin)) buf[strcspn(buf, "\n")] = '\0';
    else if (size > 0) buf[0] = '\0';
}

int readInt() {
    char line[64];
    int x;
    while (1) {
        if (!fgets(line, sizeof(line), stdin)) return 0;
        if (sscanf(line, "%d", &x) == 1) return x;
        printf("Enter valid number: ");
    }
}

float readFloat() {
    char line[64];
    float x;
    while (1) {
        if (!fgets(line, sizeof(line), stdin)) return 0.0f;
        if (sscanf(line, "%f", &x) == 1) return x;
        printf("Enter valid float: ");
    }
}

void pauseScreen() {
    printf("\nPress ENTER to continue...");
    char tmp[8];
    readLine(tmp, sizeof(tmp));
}

int managementLogin();
int studentLogin(char sid[]);
void addStudent();
void displayAll();
void updateStudent();
void deleteStudent();
void changeStudentId();
void viewTickets();
void closeTicket();
void showAnalytics();
void sortStudentsMenu();
void exportToCSV();
void viewMyDetails(char sid[]);
void raiseTicket(char sid[]);
void changePassword(char sid[]);

int managementLogin() {
    char u[32], p[32];
    printf("\n----- MANAGEMENT LOGIN -----\n");
    printf("Username: ");
    readLine(u, sizeof(u));
    printf("Password: ");
    readLine(p, sizeof(p));
    if (strcmp(u, "admin") == 0 && strcmp(p, "admin123") == 0)
        return 1;
    return 0;
}

int studentLogin(char sid[]) {
    char pass[20];
    Student s;

    printf("\n----- STUDENT LOGIN -----\n");
    printf("Enter Student ID: ");
    readLine(sid, 20);

    printf("Password: ");
    readLine(pass, sizeof(pass));

    FILE *fp = fopen(STUD_FILE, "rb");
    if (!fp) return 0;

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (strcmp(s.id, sid) == 0 && strcmp(s.password, pass) == 0) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void viewMyDetails(char sid[]) {
    Student s;
    FILE *fp = fopen(STUD_FILE, "rb");
    if (!fp) return;

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (strcmp(s.id, sid) == 0) {
            printf("\n--- MY DETAILS ---\n");
            printf("ID: %s\n", s.id);
            printf("Name: %s\n", s.name);
            printf("Branch: %s\n", s.branch);
            printf("Section: %s\n", s.section);
            printf("CGPA: %.2f\n", s.cgpa);
            printf("Phone: %s\n", s.phone);
            break;
        }
    }
    fclose(fp);
    pauseScreen();
}

void changePassword(char sid[]) {
    Student s;
    FILE *fp = fopen(STUD_FILE, "rb+");
    if (!fp) return;

    char newPass[20];
    printf("\nEnter new password: ");
    readLine(newPass, sizeof(newPass));

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (strcmp(s.id, sid) == 0) {
            strcpy(s.password, newPass);
            fseek(fp, -(long)sizeof(Student), SEEK_CUR);
            fwrite(&s, sizeof(Student), 1, fp);
            break;
        }
    }

    fclose(fp);
    printf("Password updated.\n");
    pauseScreen();
}

void raiseTicket(char sid[]) {
    Ticket t;
    FILE *fp = fopen(TKT_FILE, "ab");
    if (!fp) return;

    static int idCounter = 1;

    t.ticketId = idCounter++;
    strcpy(t.studentId, sid);

    printf("Enter your complaint: ");
    readLine(t.message, sizeof(t.message));

    strcpy(t.status, "Open");

    fwrite(&t, sizeof(Ticket), 1, fp);
    fclose(fp);

    printf("Ticket raised!\n");
    pauseScreen();
}

void addStudent() {
    Student s;
    FILE *fp = fopen(STUD_FILE, "ab");
    if (!fp) return;

    printf("\n--- ADD STUDENT ---\n");
    printf("ID: "); readLine(s.id, 20);
    printf("Name: "); readLine(s.name, 50);
    printf("Branch: "); readLine(s.branch, 20);
    printf("Section: "); readLine(s.section, 10);
    printf("CGPA: "); s.cgpa = readFloat();
    printf("Phone: "); readLine(s.phone, 15);
    printf("Password: "); readLine(s.password, 20);

    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);

    printf("Student added.\n");
    pauseScreen();
}

void displayAll() {
    Student s;
    FILE *fp = fopen(STUD_FILE, "rb");
    if (!fp) return;

    printf("\n--- STUDENT LIST ---\n");
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        printf("%s | %s | %s | %s | %.2f | %s\n",
               s.id, s.name, s.branch, s.section, s.cgpa, s.phone);
    }

    fclose(fp);
    pauseScreen();
}

void updateStudent() {
    Student s;
    char sid[20];

    printf("Enter ID to update: ");
    readLine(sid, 20);

    FILE *fp = fopen(STUD_FILE, "rb+");
    if (!fp) return;

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (strcmp(s.id, sid) == 0) {
            printf("New Name: "); readLine(s.name, 50);
            printf("New Branch: "); readLine(s.branch, 20);
            printf("New Section: "); readLine(s.section, 10);
            printf("New CGPA: "); s.cgpa = readFloat();
            printf("New Phone: "); readLine(s.phone, 15);
            fseek(fp, -(long)sizeof(Student), SEEK_CUR);
            fwrite(&s, sizeof(Student), 1, fp);
            break;
        }
    }
    fclose(fp);
    printf("Updated.\n");
    pauseScreen();
}

void deleteStudent() {
    char sid[20];
    Student s;

    printf("Enter ID to delete: ");
    readLine(sid, 20);

    FILE *fp = fopen(STUD_FILE, "rb");
    FILE *temp = fopen("temp.dat", "wb");

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (strcmp(s.id, sid) != 0)
            fwrite(&s, sizeof(Student), 1, temp);
    }

    fclose(fp); fclose(temp);

    remove(STUD_FILE);
    rename("temp.dat", STUD_FILE);

    printf("Deleted.\n");
    pauseScreen();
}

void changeStudentId() {
    char oldId[20], newId[20];
    Student s;

    printf("Old ID: ");
    readLine(oldId, 20);

    printf("New ID: ");
    readLine(newId, 20);

    FILE *fp = fopen(STUD_FILE, "rb+");
    if (!fp) return;

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (strcmp(s.id, oldId) == 0) {
            strcpy(s.id, newId);
            fseek(fp, -(long)sizeof(Student), SEEK_CUR);
            fwrite(&s, sizeof(Student), 1, fp);
            break;
        }
    }

    fclose(fp);
    printf("ID changed!\n");
    pauseScreen();
}

void viewTickets() {
    Ticket t;
    FILE *fp = fopen(TKT_FILE, "rb");
    if (!fp) return;

    printf("\n--- TICKETS ---\n");
    while (fread(&t, sizeof(Ticket), 1, fp) == 1) {
        printf("ID: %d | SID: %s | Msg: %s | Status: %s\n",
               t.ticketId, t.studentId, t.message, t.status);
    }

    fclose(fp);
    pauseScreen();
}

void closeTicket() {
    int tid;
    Ticket t;

    printf("Enter Ticket ID to close: ");
    tid = readInt();

    FILE *fp = fopen(TKT_FILE, "rb+");
    if (!fp) return;

    while (fread(&t, sizeof(Ticket), 1, fp) == 1) {
        if (t.ticketId == tid) {
            strcpy(t.status, "Closed");
            fseek(fp, -(long)sizeof(Ticket), SEEK_CUR);
            fwrite(&t, sizeof(Ticket), 1, fp);
            break;
        }
    }

    fclose(fp);
    printf("Closed.\n");
    pauseScreen();
}

void showAnalytics() {
    Student s;
    int count = 0;
    float sum = 0, max = 0, min = 10;

    FILE *fp = fopen(STUD_FILE, "rb");
    if (!fp) return;

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        count++;
        sum += s.cgpa;
        if (s.cgpa > max) max = s.cgpa;
        if (s.cgpa < min) min = s.cgpa;
    }

    fclose(fp);

    if (count == 0) printf("No data.\n");
    else {
        printf("\n--- CGPA ANALYTICS ---\n");
        printf("Total Students: %d\n", count);
        printf("Average CGPA: %.2f\n", sum / count);
        printf("Highest CGPA: %.2f\n", max);
        printf("Lowest CGPA: %.2f\n", min);
    }

    pauseScreen();
}

void exportToCSV() {
    Student s;
    FILE *fp = fopen(STUD_FILE, "rb");
    FILE *csv = fopen(CSV_FILE, "w");

    fprintf(csv, "ID,Name,Branch,Section,CGPA,Phone\n");

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        fprintf(csv, "%s,%s,%s,%s,%.2f,%s\n",
                s.id, s.name, s.branch, s.section, s.cgpa, s.phone);
    }

    fclose(fp);
    fclose(csv);

    printf("Exported.\n");
    pauseScreen();
}

void sortStudentsMenu() {
    printf("Sorting not implemented.\n");
    pauseScreen();
}

void managementMenu() {
    int c;
    while (1) {
        printf("\n----- MANAGEMENT MENU -----\n");
        printf("1. Add Student\n");
        printf("2. View All Students\n");
        printf("3. Update Student\n");
        printf("4. Delete Student\n");
        printf("5. Change Student ID\n");
        printf("6. View Tickets\n");
        printf("7. Close Ticket\n");
        printf("8. Analytics\n");
        printf("9. Sorting Options\n");
        printf("10. Export to CSV\n");
        printf("0. Logout\n");
        printf("Choose: ");
        c = readInt();

        switch (c) {
            case 1: addStudent(); break;
            case 2: displayAll(); break;
            case 3: updateStudent(); break;
            case 4: deleteStudent(); break;
            case 5: changeStudentId(); break;
            case 6: viewTickets(); break;
            case 7: closeTicket(); break;
            case 8: showAnalytics(); break;
            case 9: sortStudentsMenu(); break;
            case 10: exportToCSV(); break;
            case 0: return;
        }
    }
}

void studentMenu(char sid[]) {
    int c;
    while (1) {
        printf("\n----- STUDENT MENU -----\n");
        printf("1. View My Details\n");
        printf("2. Raise Ticket\n");
        printf("3. Change Password\n");
        printf("0. Logout\n");
        printf("Choose: ");
        c = readInt();

        switch (c) {
            case 1: viewMyDetails(sid); break;
            case 2: raiseTicket(sid); break;
            case 3: changePassword(sid); break;
            case 0: return;
        }
    }
}

int main() {
    int ch;
    char sid[20];

    while (1) {
        printf("\n===== STUDENT RECORD MANAGEMENT SYSTEM =====\n");
        printf("1. Management Login\n");
        printf("2. Student Login\n");
        printf("0. Exit\n");
        printf("Choose: ");
        ch = readInt();

        if (ch == 1) {
            if (managementLogin()) managementMenu();
            else { printf("Login failed.\n"); pauseScreen(); }
        }
        else if (ch == 2) {
            if (studentLogin(sid)) studentMenu(sid);
            else { printf("Login failed.\n"); pauseScreen(); }
        }
        else if (ch == 0) {
            printf("Exiting...\n");
            return 0;
        }
        else {
            printf("Invalid.\n");
            pauseScreen();
        }
    }
}

