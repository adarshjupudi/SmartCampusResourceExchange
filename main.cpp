#include <iostream>
#include <vector>
#include <string>
#include <limits> 

#include "Marketplace.h"
#include "User.h"
#include "Book.h"
#include "LoanTransaction.h"
#include "Electronic.h"
#include "LabGear.h"

using namespace std;

int main()
{
    Marketplace marketplace;

    // 1. Load data and synchronize all ID counters internally
    marketplace.loadUsers("users.txt");
    marketplace.loadResources("items.txt");
    marketplace.loadTransactions("transactions.txt");

    User* currentUser = nullptr;

    while(true)
    {
        if(currentUser == nullptr)
        {
            cout << "\n=== SMART CAMPUS RESOURCE EXCHANGE ===\n";
            cout << "1. Register\n";
            cout << "2. Login\n";
            cout << "0. Exit\n";
            cout << "Choice: ";

            int choice;
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

            if(choice == 0) break;

            if(choice == 1)
            {
                string name, pass;
                cout << "Enter name: ";
                cin >> name;
                cout << "Enter password: ";
                cin >> pass;
                User* u = new User(name, pass);
                marketplace.addUser(u);
                marketplace.saveUsers("users.txt");

                cout << "Registered successfully. Your ID is: " << u->getUserId() << "\n";
            }
            else if(choice == 2)
            {
                string name, pass;
                cout << "Enter name: ";
                cin >> name;
                cout << "Enter password: ";
                cin >> pass;

                bool found = false;
                for(User* u : marketplace.getUsers())
                {
                    if(u->getName() == name && u->checkPassword(pass))
                    {
                        currentUser = u;
                        found = true;
                        cout << "Login successful.\n";
                        break;
                    }
                }

                if(!found) cout << "Invalid credentials.\n";
            }
        }
        else
        {
            cout << "\nLogged in as: " << currentUser->getName() << "\n";
            cout << "Trust Points: " << currentUser->getTrustPoints() << "\n";

            cout << "\n1. Add Book\n";
            cout << "2. View Resources\n";
            cout << "3. Request Loan\n";
            cout << "4. Logout\n";
            cout << "Choice: ";

            int choice;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

            if(choice == 4)
            {
                currentUser = nullptr;
                continue;
            }

            if(choice == 1)
            {
                string title, author, isbn;
                int edition;

                cout << "Title: ";
                getline(cin, title); 
                cout << "Author: ";
                getline(cin, author);
                cout << "ISBN: ";
                cin >> isbn;
                cout << "Edition: ";
                cin >> edition;

              
                Book* b = new Book(
                    currentUser->getUserId(),
                    Resource::ImportanceLevel::MEDIUM,
                    title,
                    author,
                    isbn,
                    edition
                );

                marketplace.addResource(b);
                marketplace.saveResources("items.txt");
                cout << "Book added successfully with ID: " << b->getResourceId() << "\n";
            }
            else if(choice == 2)
            {
                cout << "\n--- Available Resources ---\n";
                for(Resource* r : marketplace.getResources())
                {
                    cout << "ID: " << r->getResourceId()
                         << " | Owner ID: " << r->getOwnerId()
                         << " | Status: "
                         << (r->getStatus() == Resource::Status::AVAILABLE ? "AVAILABLE" : "LOANED");

                    if(r->getResourceType() == "Book")
                    {
                        Book* b = dynamic_cast<Book*>(r);
                        cout << " | Type: Book | Title: " << b->getTitle() 
                             << " | Author: " << b->getAuthor();
                    }
                    cout << endl;
                }
            }
            else if(choice == 3)
            {
                int rid;
                cout << "Enter Resource ID: ";
                cin >> rid;

                Resource* target = nullptr;
                for(Resource* r : marketplace.getResources())
                {
                    if(r->getResourceId() == rid) { target = r; break; }
                }

                if(target == nullptr) {
                    cout << "Invalid resource ID.\n";
                    continue;
                }

                // Request loan (dates can be dynamic in future steps)
                LoanTransaction* tx = marketplace.requestLoan(
                    currentUser, target, "2026-01-01", "2026-01-10"
                );

                if(tx == nullptr) cout << "Loan request failed (Check trust or availability).\n";
                else cout << "Loan approved!\n";
            }
        }
    }

    cout << "Exiting program. Saving data...\n";
    marketplace.saveUsers("users.txt");
    marketplace.saveResources("items.txt");
    marketplace.saveTransactions("transactions.txt"); // Ensure transactions are saved
    
    return 0;
}