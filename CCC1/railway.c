#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SEATS 5  // change anytime!

// Booked Passenger Linked List
typedef struct BookNode {
    int id;
    char name[30];
    int age;
    char phone[15];
    int seatNo;
    struct BookNode *next;
} BookNode;

// Waiting Passenger Queue
typedef struct WaitNode {
    int id;
    char name[30];
    int age;
    char phone[15];
    struct WaitNode *next;
} WaitNode;

BookNode *bookedHead = NULL;
WaitNode *waitFront = NULL, *waitRear = NULL;

// Function Prototypes
void adminLogin();
void loadData();
void saveData();
void bookTicket();
void cancelTicket();
void displayPassengers();
void searchPassenger();
void updatePassenger();
int countBooked();
int availableSeats();
int isDuplicate(int id);
void enqueue(int id, char name[], int age, char phone[]);
void dequeueToBooked();
void sortBookedList();
void renumberSeats();   // ✅ added prototype

// ================= ADMIN LOGIN FROM FILE ==================
void adminLogin() {
    char u[20], p[20], fu[20], fp[20];
    int found = 0;

    FILE *fa = fopen("admin.txt", "r");
    if (!fa) {
        printf("❌ admin.txt missing! Create file first!\n");
        exit(0);
    }

    printf("\n========== ADMIN LOGIN ==========\n");
    printf("Username: ");  scanf("%s", u);
    printf("Password: ");  scanf("%s", p);

    // supports MULTIPLE admins in admin.txt
    while (fscanf(fa, "%s %s", fu, fp) != EOF) {
        if (strcmp(u, fu) == 0 && strcmp(p, fp) == 0) {
            found = 1;
            break;
        }
    }
    fclose(fa);

    if (found)
        printf("\n✔ Login Successful!\n");
    else {
        printf("\n❌ Invalid Login!\n");
        exit(0);
    }
}

// ================= LOAD & SAVE DATA =================
void loadData() {
    FILE *fb = fopen("booked.txt", "r");
    FILE *fw = fopen("waiting.txt", "r");

    int id, age, seat;
    char name[30], phone[15];

    if (fb) {
        while (fscanf(fb, "%d %s %d %s %d", &id, name, &age, phone, &seat) != EOF) {
            BookNode *node = malloc(sizeof(BookNode));
            node->id = id; 
            strcpy(node->name, name);
            node->age = age; 
            strcpy(node->phone, phone);
            node->seatNo = seat;
            node->next = bookedHead;
            bookedHead = node;
        }
        fclose(fb);
    }

    if (fw) {
        while (fscanf(fw, "%d %s %d %s", &id, name, &age, phone) != EOF)
            enqueue(id, name, age, phone);
        fclose(fw);
    }
}

void saveData() {
    FILE *fb = fopen("booked.txt", "w");
    FILE *fw = fopen("waiting.txt", "w");

    BookNode *b = bookedHead;
    while (b) {
        fprintf(fb, "%d %s %d %s %d\n", b->id, b->name, b->age, b->phone, b->seatNo);
        b = b->next;
    }
    fclose(fb);

    WaitNode *w = waitFront;
    while (w) {
        fprintf(fw, "%d %s %d %s\n", w->id, w->name, w->age, w->phone);
        w = w->next;
    }
    fclose(fw);
}

// ================= UTILITIES =================
int countBooked() {
    int c = 0;
    BookNode *t = bookedHead;
    while (t) { 
        c++; 
        t = t->next; 
    }
    return c;
}

int availableSeats() {
    return MAX_SEATS - countBooked();
}

int isDuplicate(int id) {
    BookNode *b = bookedHead;
    while (b) {
        if (b->id == id) return 1;
        b = b->next;
    }
    WaitNode *w = waitFront;
    while (w) {
        if (w->id == id) return 1;
        w = w->next;
    }
    return 0;
}

void enqueue(int id, char name[], int age, char phone[]) {
    WaitNode *node = malloc(sizeof(WaitNode));
    node->id = id;
    strcpy(node->name, name);
    node->age = age;
    strcpy(node->phone, phone);
    node->next = NULL;

    if (!waitRear) 
        waitFront = waitRear = node;
    else { 
        waitRear->next = node; 
        waitRear = node; 
    }
}

void dequeueToBooked() {
    if (!waitFront) return;

    WaitNode *temp = waitFront;
    waitFront = waitFront->next;
    if (!waitFront) 
        waitRear = NULL;

    BookNode *node = malloc(sizeof(BookNode));
    node->id = temp->id;
    strcpy(node->name, temp->name);
    node->age = temp->age;
    strcpy(node->phone, temp->phone);
    node->seatNo = countBooked() + 1;
    node->next = bookedHead;
    bookedHead = node;

    free(temp);
}

void sortBookedList() {
    BookNode *i, *j;
    for (i = bookedHead; i && i->next; i = i->next) {
        for (j = i->next; j; j = j->next) {
            if (i->seatNo > j->seatNo) {
                BookNode t = *i;

                i->seatNo = j->seatNo; 
                j->seatNo = t.seatNo;

                i->id = j->id; 
                j->id = t.id;

                i->age = j->age; 
                j->age = t.age;

                char tempName[30], tempPhone[15];
                strcpy(tempName, i->name);
                strcpy(i->name, j->name);
                strcpy(j->name, tempName);

                strcpy(tempPhone, i->phone);
                strcpy(i->phone, j->phone);
                strcpy(j->phone, tempPhone);
            }
        }
    }
}

// Renumber seats so that they are 1,2,3,... with no gaps
void renumberSeats() {
    sortBookedList();
    int s = 1;

    BookNode *b = bookedHead;
    while (b != NULL) {
        b->seatNo = s++;
        b = b->next;
    }
}

// ================= BOOKING =================
void bookTicket() {
    int id, age;
    char name[30], phone[15];

    printf("\nEnter Passenger ID: ");
    scanf("%d", &id);
    if (isDuplicate(id)) {
        printf("❌ ID already exists!\n");
        return;
    }

    printf("Name: "); 
    scanf("%s", name);
    printf("Age: "); 
    scanf("%d", &age);
    printf("Phone: "); 
    scanf("%s", phone);

    if (availableSeats() > 0) {
        BookNode *node = malloc(sizeof(BookNode));
        node->id = id; 
        strcpy(node->name, name);
        node->age = age; 
        strcpy(node->phone, phone);
        node->seatNo = countBooked() + 1;
        node->next = bookedHead;
        bookedHead = node;

        printf("✔ Ticket Confirmed - Seat %d\n", node->seatNo);
    } else {
        enqueue(id, name, age, phone);
        printf("⚠ Full! Added to waiting list.\n");
    }
    saveData();
}

// ================= CANCELLATION =================
void cancelTicket() {
    int id;
    printf("\nEnter Passenger ID to Cancel: ");
    scanf("%d", &id);

    // ----- Case 1: Cancel in CONFIRMED list -----
    BookNode *temp = bookedHead, *prev = NULL;
    while (temp != NULL && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }

    if (temp != NULL) {
        if (prev == NULL)
            bookedHead = temp->next;
        else
            prev->next = temp->next;

        printf("\n✔ Ticket Cancelled from Confirmed List!\n");
        free(temp);

        // Promote next waiting passenger
        if (waitFront) {
            dequeueToBooked();
            printf("⤴ Waiting passenger promoted to Confirmed List!\n");
        }

        // ✅ Renumber seats & save after cancellation and promotion
        renumberSeats();
        saveData();
        return;  // stop here
    }

    // ----- Case 2: Cancel in WAITING LIST -----
    WaitNode *wtemp = waitFront, *wprev = NULL;
    while (wtemp != NULL && wtemp->id != id) {
        wprev = wtemp;
        wtemp = wtemp->next;
    }

    if (wtemp != NULL) {
        if (wprev == NULL)
            waitFront = wtemp->next;
        else
            wprev->next = wtemp->next;

        if (wtemp == waitRear)
            waitRear = wprev;

        free(wtemp);
        printf("\n Ticket Cancelled from Waiting List!\n");

        saveData();
        return;
    }

    // ----- Not found anywhere -----
    printf("\n Passenger ID Not Found!\n");
}

// ================= DISPLAY =================
void displayPassengers() {
    sortBookedList();

    printf("\n===== CONFIRMED PASSENGERS =====\n");
    BookNode *b = bookedHead;
    if (!b) 
        printf("Empty\n");
    while (b) {
        printf("Seat:%d | ID:%d | %s | %d | %s\n",
            b->seatNo, b->id, b->name, b->age, b->phone);
        b = b->next;
    }
    printf("Seats Left: %d\n", availableSeats());

    printf("\n===== WAITING LIST =====\n");
    WaitNode *w = waitFront;
    int pos = 1;
    if (!w) 
        printf("Empty\n");
    while (w) {
        printf("Pos:%d | ID:%d | %s | %d | %s\n",
            pos++, w->id, w->name, w->age, w->phone);
        w = w->next;
    }
}

// ================= SEARCH =================
void searchPassenger() {
    int id;
    printf("\nEnter ID: ");
    scanf("%d", &id);

    BookNode *b = bookedHead;
    while (b) {
        if (b->id == id) {
            printf("\n Found in CONFIRMED\n");
            printf("Seat:%d | %s | %d | %s\n",
                b->seatNo, b->name, b->age, b->phone);
            return;
        }
        b = b->next;
    }

    WaitNode *w = waitFront;
    int pos = 1;
    while (w) {
        if (w->id == id) {
            printf("\n Found in WAITING (Pos:%d)\n", pos);
            printf("%s | %d | %s\n", w->name, w->age, w->phone);
            return;
        }
        pos++; 
        w = w->next;
    }

    printf(" Passenger not found!\n");
}

// ================= UPDATE =================
void updatePassenger() {
    int id, c;
    printf("\nEnter Passenger ID to Update: ");
    scanf("%d", &id);

    BookNode *b = bookedHead;
    while (b != NULL) {
        if (b->id == id) {
            printf("\n✔ Passenger Found in CONFIRMED List\n");
            printf("Choose field to update:\n");
            printf("1. Name\n2. Age\n3. Phone\nChoice: ");
            scanf("%d", &c);

            switch(c) {
                case 1:
                    printf("Enter New Name: ");
                    scanf("%s", b->name);
                    break;

                case 2:
                    printf("Enter New Age: ");
                    scanf("%d", &b->age);
                    break;

                case 3:
                    printf("Enter New Phone: ");
                    scanf("%s", b->phone);
                    break;

                default:
                    printf(" Invalid option!\n");
                    return;
            }

            printf("✔ Updated Successfully!\n");
            saveData();
            return;   // STOP HERE
        }
        b = b->next;
    }

    WaitNode *w = waitFront;
    while (w != NULL) {
        if (w->id == id) {
            printf("\n✔ Passenger Found in WAITING List\n");
            printf("Choose field to update:\n");
            printf("1. Name\n2. Age\n3. Phone\nChoice: ");
            scanf("%d", &c);

            switch(c) {
                case 1:
                    printf("Enter New Name: ");
                    scanf("%s", w->name);
                    break;

                case 2:
                    printf("Enter New Age: ");
                    scanf("%d", &w->age);
                    break;

                case 3:
                    printf("Enter New Phone: ");
                    scanf("%s", w->phone);
                    break;

                default:
                    printf(" Invalid option!\n");
                    return;
            }

            printf("✔ Updated Successfully!\n");
            saveData();
            return;   // STOP HERE
        }
        w = w->next;
    }

    printf(" Passenger Not Found!\n");
}

// ================= MAIN MENU =================
int main() {
    int ch;
    loadData();
    adminLogin();

    while (1) {
        printf("\n===== ADMIN PANEL =====\n");
        printf("1.Book Ticket\n2.Cancel Ticket\n3.Display Lists\n");
        printf("4.Search Passenger\n5.Update Details\n6.Exit\n");
        printf("Choice: ");
        scanf("%d", &ch);

        switch(ch) {
            case 1: bookTicket(); break;
            case 2: cancelTicket(); break;
            case 3: displayPassengers(); break;
            case 4: searchPassenger(); break;
            case 5: updatePassenger(); break;
            case 6:
                saveData();
                printf("\nExiting System... Goodbye!\n");
                return 0;   // Stops infinite loop

            default: 
                printf("Invalid!\n");
        }
    }
}
