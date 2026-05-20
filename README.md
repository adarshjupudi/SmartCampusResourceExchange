# Smart Campus Resource Exchange System

A C++ based peer-to-peer campus resource sharing system that allows users to list, borrow, return, and manage shared resources such as books, electronics, and lab equipment.

## Key Features

- User registration and login
- Resource listing and management
- Support for books, electronics, and lab gear
- Trust-score based borrowing rules
- Owner-controlled lending policies
- Loan request and return workflow
- Due date calculation using C++ chrono
- File-based data persistence
- Notification system
- Condition evidence tracking
- Dispute resolution for damaged items
- UTF-8 console support

## Technical Highlights

### Polymorphic Resource Management

The system uses a common Resource base class with derived classes such as Book, Electronic, and LabGear. This allows different types of resources to be stored and managed through a common structure while still supporting type-specific details.

### Transaction-Based Dispute Handling

Loan-related disputes are handled inside the LoanTransaction flow. When damage is reported, the system tracks evidence, updates the transaction state, and adjusts trust points based on the final decision.

### File-Based Persistence

The system stores data in plain text files:

- users.txt
- items.txt
- transactions.txt
- notifications.txt

It uses pipe delimiters (|) for main fields and comma delimiters (,) for nested values such as evidence image paths.

### Static ID Recovery

On startup, the system scans existing files and restores ID counters based on the highest saved IDs. This prevents duplicate IDs after restarting the program.

### Date and Time Handling

The system uses the C++ chrono library to calculate loan deadlines and manage due dates.

### UTF-8 Console Support

On Windows, UTF-8 console output is enabled to correctly display formatted terminal UI characters.

## Core File Database Layouts

### users.txt

1|Alice|SecurePass123|50
2|Bob|PassWord456|-10

### items.txt

BOOK|100|1|Design Patterns|Gang of Four|978-0201633610|1|AVAILABLE|0|14
ELECTRONIC|101|1|Lab Laptop|Dell|XPS-15|1|1|LOANED|30|7
LABGEAR|102|2|Hazmat Apron|Vinyl-Safe|5|1|OVERDUE|70|3

### transactions.txt

LOAN|1|2|101|2026-05-10|2026-05-17|COMPLETED|img/on_1.png|img/ret_1.png|0||0
LOAN|2|2|102|2026-05-12|2026-05-15|DISPUTED|ok.jpg|torn.jpg|1|Apron strap torn off|0

## Setup and Execution

### Prerequisites

Make sure a C++ compiler such as GCC/G++ with C++11 or C++14 support is installed.

### Clone the Repository

git clone https://github.com/YOUR_USERNAME/YOUR_REPOSITORY.git
cd YOUR_REPOSITORY

### Create Required Files

Create these files in the project folder if they do not already exist:

users.txt
items.txt
transactions.txt
notifications.txt

### Compile

g++ -std=c++14 *.cpp -o SmartCampus.exe

### Run

.\SmartCampus.exe

## Usage Guide

### User Registration

Register a new user and verify that the user ID is stored in users.txt. Restart the program and register another user to confirm that ID counters continue correctly.

### Add Resources

Log in and add resources such as books, electronics, or lab gear. Each resource type stores its own specific details.

### Borrow Resources

Users can request available resources. The system checks trust-score requirements and lending rules before allowing a loan.

### Return Resources

Borrowers can return items and provide condition evidence such as image paths.

### Notifications

The system displays pending notifications for loan updates, overdue items, and other important events.

### Dispute Resolution

If an item is damaged, the owner can raise a dispute. The system compares pre-loan and return evidence, updates the transaction status, and adjusts trust scores fairly.

## Future Improvements

- Add graphical user interface
- Replace text files with a database
- Add search and filter options
- Add email or push notifications
- Add admin dashboard
- Add second-hand selling support
