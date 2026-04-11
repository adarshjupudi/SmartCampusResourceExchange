#include <iostream>
#include <vector>
#include <string>
#include <limits> 
#include <chrono>
#include <iomanip>
#include <sstream>

#include "Marketplace.h"
#include "User.h"
#include "Book.h"
#include "LoanTransaction.h"
#include "Electronic.h"
#include "LabGear.h"

using namespace std;

// Helper to get actual current date in YYYY-MM-DD
string getCurrentDate()
{
    auto now = chrono::system_clock::now();
    time_t in_time_t = chrono::system_clock::to_time_t(now);
    stringstream ss;
    ss << put_time(localtime(&in_time_t), "%Y-%m-%d");
    return ss.str();
}

int main()
{
    Marketplace marketplace;
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
            if (!(cin >> choice)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
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
                const std::vector<User*>& users = marketplace.getUsers();
                for (size_t i = 0; i < users.size(); ++i) 
                {
                    if (users[i]->getName() == n && users[i]->checkPassword(p)) 
                    {
                        currentUser = users[i];
                        marketplace.updateOverdueStatus(getCurrentDate()); // Dynamic date
                        
                        auto notes = marketplace.getNotifications(currentUser->getUserId());
                        if (!notes.empty())
                        {
                            cout << "\n--- UNREAD NOTIFICATIONS ---\n";
                            for (size_t j = 0; j < notes.size(); ++j) cout << "[!] " << notes[j] << endl;
                            marketplace.clearNotifications(currentUser->getUserId());
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
                const std::vector<Resource*>& res = marketplace.getResources();
                for (size_t i = 0; i < res.size(); ++i) 
                {
                    string s = (res[i]->getStatus() == Resource::Status::AVAILABLE) ? "AVAILABLE" : 
                               (res[i]->getStatus() == Resource::Status::OVERDUE ? "OVERDUE" : "LOANED");
                    cout << "ID: " << res[i]->getResourceId() << " | Status: " << s;
                    if (res[i]->getResourceType() == "Book") 
                    { 
                        Book* b = dynamic_cast<Book*>(res[i]); cout << " | Book: " << b->getTitle(); 
                    }
                    cout << endl;
                }
            } 
            else if (choice == 3) 
            {
                int rid; cout << "Enter Resource ID: "; cin >> rid;
                Resource* target = nullptr;
                const std::vector<Resource*>& res = marketplace.getResources();
                for (size_t i = 0; i < res.size(); ++i) if (res[i]->getResourceId() == rid) target = res[i];
                if (!target) { cout << "Invalid ID.\n"; continue; }
                
                if (marketplace.requestLoan(currentUser, target, getCurrentDate(), "2026-04-20")) cout << "Loan approved!\n";
                else cout << "Request failed.\n";
            } 
            else if (choice == 4) 
            {
                int rid; cout << "Enter Resource ID to return: "; cin >> rid;
                LoanTransaction* active = nullptr;
                const std::vector<LoanTransaction*>& txs = marketplace.getTransactions();
                for (size_t i = 0; i < txs.size(); ++i) if (txs[i]->getResource()->getResourceId() == rid && txs[i]->getBorrower()->getUserId() == currentUser->getUserId() && !txs[i]->isReturned()) active = txs[i];
                if (active) 
                {
                    active->markReturned(getCurrentDate());
                    cout << "Resource returned successfully.\n";
                    marketplace.saveResources("items.txt"); marketplace.saveTransactions("transactions.txt");
                } 
                else cout << "No active loan found.\n";
            }
            else if (choice == 5)
            {
                auto notes = marketplace.getNotifications(currentUser->getUserId());
                if (notes.empty()) cout << "No notifications.\n";
                else
                {
                    for (size_t i = 0; i < notes.size(); ++i) cout << ">> " << notes[i] << endl;
                    marketplace.clearNotifications(currentUser->getUserId());
                }
            }
        }
    }
    marketplace.saveUsers("users.txt"); marketplace.saveResources("items.txt"); marketplace.saveTransactions("transactions.txt"); marketplace.saveNotifications("notifications.txt");
    return 0;
}