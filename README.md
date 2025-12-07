# Railway Reservation & Management System (C Program)

A complete Railway Ticket Reservation System implemented in C programming language using core data structures. This system ensures efficient booking, cancellation, and passenger management with permanent storage using text files.

---

## Overview

This project is designed to simulate a basic railway ticket reservation process with the following major features:

- Admin authentication using external credentials file  
- Linked List for confirmed bookings  
- Queue for managing waiting passengers  
- Automatic seat allocation and promotion from waiting list  
- Searching, updating, and deleting passenger records  
- Permanently stored data using text files  
- Compatible with Windows, Linux, and macOS  
- Fully console-based and lightweight  

---

## Key Features

### Admin Login System
- Credentials stored in a text file (`admin.txt`)
- Only authorized admin can access the system
- Simple authentication with username and password

### Ticket Management
| Operation | Availability |
|----------|:-----------:|
| Book Ticket | Yes |
| Cancel Ticket | Yes |
| Display Passenger Lists | Yes |
| Search Passenger | Yes |
| Update Passenger Details | Yes |
| Promote Waiting Passenger automatically | Yes |

---

## Data Storage Format

### Confirmed Passengers (booked.txt)
Each confirmed ticket entry contains:
id name age phone seatNo



Example:
101 Nikhil 20 9876543210 1
102 Manoj 21 9123456780 2



### Waiting Passengers (waiting.txt)
Each waiting entry contains:
id name age phone



Example:
201 Surya 22 9876578908
202 Bala 19 9123456780



### Admin Credentials (admin.txt)
username password



Example:
admin 1234



---

## Menu Options

Admin Control Panel:
1. Book Ticket  
2. Cancel Ticket  
3. Display Passenger Lists  
4. Search Passenger  
5. Update Passenger Details  
6. Exit  

---

## Project Structure

Railway-Reservation-System/
│
├── railway.c
├── admin.txt
├── booked.txt (auto-generated)
├── waiting.txt (auto-generated)
└── README.md



---

## How to Run

### Windows
gcc railway.c -o railway.exe
railway.exe



### Linux / macOS
gcc railway.c -o railway
./railway



---

## Conceptual Implementation

| Feature | Data Structure | Reason |
|--------|----------------|-------|
| Confirmed bookings | Linked List | Dynamic allocation of seats |
| Waiting list | Queue | FIFO-based ticket movement |
| Storage | Text Files | Permanent data retention |

---

## Advantages

- Saves ticket data permanently  
- Waiting management handled efficiently  
- Suitable for academic project submission  
- Uses DSA concepts practically  
- Scalable and customizable  

---

## Future Improvements

- Support for multiple trains and classes  
- Enhanced UI design with color formatting  
- Detailed ticket receipt printing  
- Database integration (MySQL / SQLite)  
- Password encryption for better security 
