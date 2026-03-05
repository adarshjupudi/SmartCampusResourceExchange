#include <iostream>
#include <vector>
#include <string>

#include "Marketplace.h"
#include "User.h"
#include "Book.h"
#include "LoanTransaction.h"
#include "Book.h"
#include "Electronic.h"
#include "LabGear.h"
using namespace std;

int main()
{
    Marketplace marketplace;
    vector<User*> allUsers;
    vector<Resource*> allResources;

    //load users
    marketplace.loadUsers("users.txt");
    // copy loaded users into local list
    for(User* u : marketplace.getUsers())
    {
        allUsers.push_back(u);
    }
    int nextUserId = 1;//placeholders currently
    int nextResourceId = 100;

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
            cin >> choice;

            if(choice == 0) break;

            if(choice == 1)
            {
                string name, pass;
                cout << "Enter name: ";
                cin >> name;
                cout << "Enter password: ";
                cin >> pass;

                User* u = new User(nextUserId++, name, pass);
                allUsers.push_back(u);
                marketplace.addUser(u);

                marketplace.saveUsers("users.txt");

                cout << "Registered successfully.\n";
            }
            else if(choice == 2)
            {
                string name, pass;
                cout << "Enter name: ";
                cin >> name;
                cout << "Enter password: ";
                cin >> pass;

                bool found = false;
                for(User* u : allUsers)
                {
                    if(u->getName() == name && u->checkPassword(pass))
                    {
                        currentUser = u;
                        found = true;
                        cout << "Login successful.\n";
                        break;
                    }
                }

                if(!found)
                {
                    cout << "Invalid credentials.\n";
                }
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
                cin >> title;
                cout << "Author: ";
                cin >> author;
                cout << "ISBN: ";
                cin >> isbn;
                cout << "Edition: ";
                cin >> edition;

                Book* b = new Book(
                    nextResourceId++,
                    currentUser->getUserId(),
                    Resource::ImportanceLevel::MEDIUM,
                    title,
                    author,
                    isbn,
                    edition
                );

                allResources.push_back(b);
                marketplace.addResource(b);

                cout << "Book added successfully.\n";
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

                        cout << " | Type: Book"
                            << " | Title: " << b->getTitle()
                            << " | Author: " << b->getAuthor()
                            << " | ISBN: " << b->getIsbn()
                            << " | Edition: " << b->getEdition();
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
                for(Resource* r : allResources)
                {
                    if(r->getResourceId() == rid)
                    {
                        target = r;
                        break;
                    }
                }

                if(target == nullptr)
                {
                    cout << "Invalid resource ID.\n";
                    continue;
                }

                LoanTransaction* tx = marketplace.requestLoan(
                    currentUser,
                    target,
                    "2026-01-01",
                    "2026-01-10"
                );

                if(tx == nullptr)
                {
                    cout << "Loan request failed.\n";
                }
                else
                {
                    cout << "Loan approved.\n";
                }
            }
        }
    }
    cout << "Exiting program.\n";
    marketplace.saveUsers("users.txt");
    return 0;
}
