#include <iostream>
#include <vector>
#include <string>
#include <limits> 
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>

#include "Marketplace.h"
#include "User.h"
#include "Book.h"
#include "LoanTransaction.h"
#include "Electronic.h"
#include "LabGear.h"

using namespace std;

string getCurrentDate()
{
    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);
    tm* now_tm = localtime(&now_c);
    stringstream ss;
    ss << put_time(now_tm, "%Y-%m-%d");
    return ss.str();
}

string getFutureDate(int daysInFuture)
{
    auto now = chrono::system_clock::now();
    auto future = now + chrono::hours(24 * daysInFuture);
    time_t future_c = chrono::system_clock::to_time_t(future);
    tm* future_tm = localtime(&future_c);
    stringstream ss;
    ss << put_time(future_tm, "%Y-%m-%d");
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
            if (!(cin >> choice))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

            if (choice == 0)
            {
                break;
            }
            if (choice == 1) 
            {
                string n, p;
                cout << "Enter name: "; cin >> n; cout << "Enter password: "; cin >> p;
                User* u = new User(n, p); marketplace.addUser(u); marketplace.saveUsers("users.txt");
                cout << "Registered successfully. ID: " << u->getUserId() << "\n";
            } 
            else if (choice == 2) 
            {
                string n, p;
                cout << "Enter name: "; cin >> n; cout << "Enter password: "; cin >> p;
                const std::vector<User*>& users = marketplace.getUsers();
                for (size_t i = 0; i < users.size(); ++i) 
                {
                    if (users[i]->getName() == n && users[i]->checkPassword(p)) 
                    {
                        currentUser = users[i];
                        marketplace.updateOverdueStatus(getCurrentDate());
                        
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
            cout << "1. Add Book\n2. View Resources\n3. Request Loan\n4. Return Resource\n5. View Inbox\n6. Add Pre-Loan Evidence\n7. Set Resource Rules\n8. File Item Dispute\n9. Resolve Existing Dispute (Admin)\n10. Logout\nChoice: ";
            int choice; cin >> choice; cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (choice == 10)
            {
                currentUser = nullptr;
                continue;
            }
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
                    cout << "ID: " << res[i]->getResourceId() << " | Status: " << s 
                         << " | Req Trust: " << res[i]->getMinTrustRequired() 
                         << " | Max Days: " << res[i]->getMaxLoanDuration();
                    if (res[i]->getResourceType() == "Book") 
                    { 
                        Book* b = dynamic_cast<Book*>(res[i]);
                        cout << " | Book: " << b->getTitle(); 
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
                
                string start = getCurrentDate();
                string due = getFutureDate(target->getMaxLoanDuration());
                
                if (marketplace.requestLoan(currentUser, target, start, due))
                {
                    cout << "Loan approved! Due date dynamically set to: " << due << "\n";
                }
                else
                {
                    cout << "Request failed (Check item availability or your trust tier).\n";
                }
            } 
            else if (choice == 4) 
            {
                int rid; cout << "Enter Resource ID to return: "; cin >> rid;
                LoanTransaction* active = nullptr;
                const std::vector<LoanTransaction*>& txs = marketplace.getTransactions();
                for (size_t i = 0; i < txs.size(); ++i)
                {
                    if (txs[i]->getResource()->getResourceId() == rid && txs[i]->getBorrower()->getUserId() == currentUser->getUserId() && !txs[i]->isReturned())
                    {
                        active = txs[i];
                    }
                }
                if (active) 
                {
                    active->markReturned(getCurrentDate());
                    cout << "Resource marked returned successfully.\n";
                    
                    string evPath;
                    cout << "Enter post-return condition image/file path (or type 'done' to skip): ";
                    while (getline(cin, evPath) && evPath != "done")
                    {
                        if (!evPath.empty())
                        {
                            active->addPostReturnEvidence(evPath);
                            cout << "Evidence added. Enter another path or type 'done': ";
                        }
                    }
                    
                    marketplace.saveResources("items.txt");
                    marketplace.saveTransactions("transactions.txt");
                    cout << "Return transaction closed completely.\n";
                } 
                else
                {
                    cout << "No active loan found.\n";
                }
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
            else if (choice == 6)
            {
                cout << "\n--- Active Loans on Your Items ---\n";
                const std::vector<LoanTransaction*>& txs = marketplace.getTransactions();
                bool foundAny = false;
                for (size_t i = 0; i < txs.size(); ++i)
                {
                    if (txs[i]->getOwner()->getUserId() == currentUser->getUserId() && !txs[i]->isReturned())
                    {
                        cout << "Tx ID: " << txs[i]->getTransactionId() << " | Item ID: " << txs[i]->getResource()->getResourceId() << " | Borrower ID: " << txs[i]->getBorrower()->getUserId() << "\n";
                        foundAny = true;
                    }
                }
                if (!foundAny)
                {
                    cout << "No active outgoing loans found.\n";
                    continue;
                }
                
                int targetTxId;
                cout << "Enter Transaction ID to append pre-loan evidence to: ";
                cin >> targetTxId;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                LoanTransaction* targetTx = nullptr;
                for (size_t i = 0; i < txs.size(); ++i)
                {
                    if (txs[i]->getTransactionId() == targetTxId && txs[i]->getOwner()->getUserId() == currentUser->getUserId())
                    {
                        targetTx = txs[i];
                    }
                }
                
                if (targetTx)
                {
                    string evPath;
                    cout << "Enter pre-loan condition image/file path (or type 'done' to stop): ";
                    while (getline(cin, evPath) && evPath != "done")
                    {
                        if (!evPath.empty())
                        {
                            targetTx->addPreLoanEvidence(evPath);
                            cout << "Evidence logged. Enter another path or type 'done': ";
                        }
                    }
                    marketplace.saveTransactions("transactions.txt");
                    cout << "Pre-loan evidence saved.\n";
                }
                else
                {
                    cout << "Invalid Transaction ID or permissions denied.\n";
                }
            }
            else if (choice == 7) 
            {
                cout << "\n--- Your Listed Resources ---\n";
                const std::vector<Resource*>& res = marketplace.getResources();
                bool ownsItems = false;
                for (size_t i = 0; i < res.size(); ++i)
                {
                    if (res[i]->getOwnerId() == currentUser->getUserId())
                    {
                        cout << "ID: " << res[i]->getResourceId() << " | Name: " << res[i]->getDisplayName() 
                             << " | Current Trust Req: " << res[i]->getMinTrustRequired() 
                             << " | Max Loan Days: " << res[i]->getMaxLoanDuration() << "\n";
                        ownsItems = true;
                    }
                }
                
                if (!ownsItems)
                {
                    cout << "You have not listed any resources yet.\n";
                    continue;
                }

                int targetId;
                cout << "Enter Resource ID to modify rules for: ";
                cin >> targetId;
                
                Resource* targetRes = nullptr;
                for (size_t i = 0; i < res.size(); ++i)
                {
                    if (res[i]->getResourceId() == targetId && res[i]->getOwnerId() == currentUser->getUserId())
                    {
                        targetRes = res[i];
                    }
                }

                if (targetRes)
                {
                    int newTrust, newDuration;
                    cout << "Enter Minimum Trust Points Required to Borrow: ";
                    cin >> newTrust;
                    cout << "Enter Maximum Loan Duration Allowed (Days): ";
                    cin >> newDuration;

                    targetRes->setMinTrustRequired(newTrust);
                    targetRes->setMaxLoanDuration(newDuration);

                    marketplace.saveResources("items.txt");
                    cout << "Resource parameters modified and persisted successfully!\n";
                }
                else
                {
                    cout << "Invalid Resource ID or access denied.\n";
                }
            }
            else if (choice == 8) // NEW: File Item Dispute
            {
                cout << "\n--- Historic/Active Outgoing Loans on Your Items ---\n";
                const std::vector<LoanTransaction*>& txs = marketplace.getTransactions();
                bool foundAny = false;
                for (size_t i = 0; i < txs.size(); ++i)
                {
                    if (txs[i]->getOwner()->getUserId() == currentUser->getUserId() && !txs[i]->isDisputed())
                    {
                        string statStr = (txs[i]->getStatus() == Transaction::Status::COMPLETED) ? "COMPLETED" : "ACTIVE";
                        cout << "Tx ID: " << txs[i]->getTransactionId() << " | Item: " << txs[i]->getResource()->getDisplayName() 
                             << " | Borrower ID: " << txs[i]->getBorrower()->getUserId() << " | Status: " << statStr << "\n";
                        foundAny = true;
                    }
                }

                if (!foundAny)
                {
                    cout << "No items available to file a dispute against.\n";
                    continue;
                }

                int targetTxId;
                cout << "Enter Transaction ID to flag as DISPUTED: ";
                cin >> targetTxId;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                LoanTransaction* targetTx = nullptr;
                for (size_t i = 0; i < txs.size(); ++i)
                {
                    if (txs[i]->getTransactionId() == targetTxId && txs[i]->getOwner()->getUserId() == currentUser->getUserId())
                    {
                        targetTx = txs[i];
                    }
                }

                if (targetTx)
                {
                    string reason;
                    cout << "Enter clear reason for the asset dispute (e.g., Missing battery, Scratched screen): ";
                    getline(cin, reason);

                    targetTx->raiseDispute(reason);
                    marketplace.saveTransactions("transactions.txt");
                    marketplace.saveUsers("users.txt");
                    cout << "Dispute logged successfully. Trust modifications locked for administration audit.\n";
                }
                else
                {
                    cout << "Invalid Transaction ID or permissions validation failed.\n";
                }
            }
            else if (choice == 9) // NEW: Resolve Existing Dispute (Admin Simulation)
            {
                cout << "\n--- Active Unresolved Campus Disputes ---\n";
                const std::vector<LoanTransaction*>& txs = marketplace.getTransactions();
                bool foundAny = false;
                for (size_t i = 0; i < txs.size(); ++i)
                {
                    if (txs[i]->isDisputed() && !txs[i]->isDisputeResolved())
                    {
                        cout << "Tx ID: " << txs[i]->getTransactionId() << " | Item: " << txs[i]->getResource()->getDisplayName() 
                             << " | Owner ID: " << txs[i]->getOwner()->getUserId() << " | Borrower ID: " << txs[i]->getBorrower()->getUserId() 
                             << "\n   Reason logged: " << txs[i]->getDisputeReason() << "\n";
                        
                        // Render image paths logged in Step 6 for visual proof audit
                        cout << "   [Audit Evidence] Pre-loan links: ";
                        const auto& pre = txs[i]->getPreLoanEvidence();
                        for (size_t k = 0; k < pre.size(); ++k) cout << pre[k] << " ";
                        cout << "\n   [Audit Evidence] Post-return links: ";
                        const auto& post = txs[i]->getPostReturnEvidence();
                        for (size_t k = 0; k < post.size(); ++k) cout << post[k] << " ";
                        cout << "\n---------------------------------------\n";
                        foundAny = true;
                    }
                }

                if (!foundAny)
                {
                    cout << "No outstanding disputes found in system space.\n";
                    continue;
                }

                int targetTxId;
                cout << "Enter Transaction ID to issue judicial settlement for: ";
                cin >> targetTxId;

                LoanTransaction* targetTx = nullptr;
                for (size_t i = 0; i < txs.size(); ++i)
                {
                    if (txs[i]->getTransactionId() == targetTxId && txs[i]->isDisputed() && !txs[i]->isDisputeResolved())
                    {
                        targetTx = txs[i];
                    }
                }

                if (targetTx)
                {
                    int faultChoice;
                    cout << "Based on evidence audit, is the Borrower at fault for asset damage?\n1. Yes (Borrower Penalized)\n0. No (Owner Penalized for Fraud)\nSelect: ";
                    cin >> faultChoice;

                    targetTx->resolveDispute(faultChoice == 1);
                    marketplace.saveTransactions("transactions.txt");
                    marketplace.saveUsers("users.txt");
                    cout << "Dispute record settled permanently and trust pools recalculated.\n";
                }
                else
                {
                    cout << "Invalid Transaction ID or dispute record already finalized.\n";
                }
            }
        }
    }
    marketplace.saveUsers("users.txt");
    marketplace.saveResources("items.txt");
    marketplace.saveTransactions("transactions.txt");
    marketplace.saveNotifications("notifications.txt");
    return 0;
}