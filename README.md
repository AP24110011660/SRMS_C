# SRMS_C

#  Student Record Management System (SRMS) – C Language Project

This project is a complete **Student Record Management System** implemented in **C**, featuring persistent storage, authentication, student services, ticketing, analytics, sorting, and automatic CSV/TXT exports.  
The application is fully terminal-based and uses `.dat` binary files for fast storage and retrieval.

---

##  Core Features

### 1. Dual Login System
- **Management Login**
  - Username: `admin`
  - Password: `admin123`
- **Student Login**
  - Students authenticate using:
    - Registered ID
    - Password stored in the database (default format: `ID@pass`)

---

##  Data Storage Files

The system uses multiple persistent files:

| File | Purpose |
|------|---------|
| `students.dat` | Stores all student records (binary) |
| `tickets.dat`  | Stores student service/support tickets |
| `students.csv` | Auto-generated CSV export of all students |
| `students.txt` | Text export when viewing all students |

Data is preserved between program runs.

---

##  Student Data Structure

Each student record contains:

```c
typedef struct {
    char id[20];
    char name[50];
    char branch[20];
    char section[10];
    float cgpa;
    char phone[15];
    char password[20];
} Student;
