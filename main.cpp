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
    
    // Initial data loading
    marketplace.loadUsers("users.txt");
    marketplace.loadResources("items.txt");
    marketplace.loadTransactions("transactions.txt");
    marketplace.loadNotifications("notifications.txt");

    User* currentUser = nullptr;

    while (true)
    {
        if (currentUser == nullptr)
        {
            cout << "\n=== SMART CAMPUS RESOURCE EXCHANGE ===\n";
            cout << "1. Register\n2. Login\n0. Exit\nChoice: ";
            int choice;
            if (!(cin >> choice))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

            if (choice == 0) break;

            if (choice == 1)
            {
                string n, p; cout << "Enter name: "; cin >> n; cout << "Enter password: "; cin >> p;
                User* u = new User(n, p); marketplace.addUser(u); marketplace.saveUsers("users.txt");
                cout << "Registered successfully. ID: " << u->getUserId() << "\n";
            }
            else if (choice == 2)
            {
                string n, p; cout << "Enter name: "; cin >> n; cout << "Enter password: "; cin >> p;
                for (User* u : marketplace.getUsers())
                {
                    if (u->getName() == n && u->checkPassword(p))
                    {
                        currentUser = u;
                        marketplace.updateOverdueStatus("2026-04-04");
                        
                        // Check for persistent notifications on login
                        auto notes = marketplace.getNotifications(u->getUserId());
                        if (!notes.empty())
                        {
                            cout << "\n--- UNREAD NOTIFICATIONS ---\n";
                            for (const auto& note : notes) cout << "[!] " << note << endl;
                            marketplace.clearNotifications(u->getUserId());
                            cout << "---------------------------\n";
                        }
                        break;
                    }
                }
                if (!currentUser) cout << "Invalid credentials.\n";
            }
        }
        else
        {
            cout << "\nLogged in: " << currentUser->getName() << " | Trust: " << currentUser->getTrustPoints() << "\n";
            cout << "1. Add Book\n2. View Resources\n3. Request Loan\n4. Return Resource\n5. View Inbox\n6. Logout\nChoice: ";
            int choice; cin >> choice; cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (choice == 6) { currentUser = nullptr; continue; }
            
            if (choice == 1)
            {
                string t, a, i; int e;
                cout << "Title: "; getline(cin, t); cout << "Author: "; getline(cin, a); cout << "ISBN: "; cin >> i; cout << "Edition: "; cin >> e;
                Book* b = new Book(currentUser->getUserId(), Resource::ImportanceLevel::MEDIUM, t, a, i, e);
                marketplace.addResource(b); marketplace.saveResources("items.txt");
                cout << "Book added. ID: " << b->getResourceId() << "\n";
            }
            else if (choice == 2)
            {
                for (Resource* r : marketplace.getResources())
                {
                    string s = (r->getStatus() == Resource::Status::AVAILABLE) ? "AVAILABLE" : 
                               (r->getStatus() == Resource::Status::OVERDUE ? "OVERDUE" : "LOANED");
                    cout << "ID: " << r->getResourceId() << " | Status: " << s;
                    if (r->getResourceType() == "Book")
                    {
                        Book* b = dynamic_cast<Book*>(r);
                        cout << " | Book: " << b->getTitle();
                    }
                    cout << endl;
                }
            }
            else if (choice == 3)
            {
                int rid; cout << "Enter Resource ID: "; cin >> rid;
                Resource* target = nullptr;
                for (Resource* r : marketplace.getResources()) if (r->getResourceId() == rid) target = r;
                if (!target) { cout << "Invalid ID.\n"; continue; }
                if (marketplace.requestLoan(currentUser, target, "2026-04-01", "2026-04-10")) cout << "Loan approved!\n";
                else cout << "Request failed.\n";
            }
            else if (choice == 4)
            {
                int rid; cout << "Enter Resource ID to return: "; cin >> rid;
                LoanTransaction* active = nullptr;
                for (LoanTransaction* t : marketplace.getTransactions())
                {
                    if (t->getResource()->getResourceId() == rid && t->getBorrower()->getUserId() == currentUser->getUserId() && !t->isReturned()) active = t;
                }
                if (active)
                {
                    active->markReturned("2026-04-04");
                    cout << "Resource returned successfully.\n";
                    marketplace.saveResources("items.txt"); marketplace.saveTransactions("transactions.txt");
                }
                else cout << "No active loan found.\n";
            }
            else if (choice == 5)
            {
                auto notes = marketplace.getNotifications(currentUser->getUserId());
                if (notes.empty()) cout << "Your inbox is empty.\n";
                else
                {
                    cout << "\n--- YOUR INBOX ---\n";
                    for (const auto& note : notes) cout << ">> " << note << endl;
                    marketplace.clearNotifications(currentUser->getUserId());
                }
            }
        }
    }
    
    // Save all data including notifications before closing
    marketplace.saveUsers("users.txt");
    marketplace.saveResources("items.txt");
    marketplace.saveTransactions("transactions.txt");
    marketplace.saveNotifications("notifications.txt"); 
    
    return 0;
}