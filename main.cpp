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

// ==========================================
//           UI presentation helpers
// ==========================================

void printHeader(const string& title)
{
    cout << "\n" << string(60, '=') << "\n";
    int padding = (60 - title.length()) / 2;
    if (padding > 0)
    {
        cout << string(padding, ' ');
    }
    cout << title << "\n";
    cout << string(60, '=') << "\n";
}

void printDivider()
{
    cout << string(60, '-') << "\n";
}

void printUserPanel(User* user)
{
    if (user == nullptr)
    {
        return;
    }
    cout << "\n╔══════════════════════════════════════════════════════════╗\n";
    cout << "║ ACTIVE SESSION                                           ║\n";
    printDivider();
    cout << "  User Profile : " << user->getName() << " (ID: " << user->getUserId() << ")\n";
    cout << "  Trust Rating : " << user->getTrustPoints() << " Points\n";
    cout << "╚══════════════════════════════════════════════════════════╝\n";
}

// ==========================================
//          Dynamic Date Utilities
// ==========================================

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

// ==========================================
//               Main Execution
// ==========================================

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
            printHeader("SMART CAMPUS RESOURCE EXCHANGE");
            cout << "  [1] Account Registration\n";
            cout << "  [2] Secure User Login\n";
            cout << "  [0] Exit Application\n";
            printDivider();
            cout << "Select Operation: ";
            
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
                printHeader("ACCOUNT REGISTRATION");
                string n, p;
                cout << "Enter preferred username: "; 
                cin >> n; 
                cout << "Enter secure password  : "; 
                cin >> p;
                
                User* u = new User(n, p); 
                marketplace.addUser(u); 
                marketplace.saveUsers("users.txt");
                
                printDivider();
                cout << ">> Registration successful!\n";
                cout << ">> Allocated System ID: " << u->getUserId() << "\n";
            } 
            else if (choice == 2) 
            {
                printHeader("SECURE SYSTEM LOGIN");
                string n, p;
                cout << "Username: "; 
                cin >> n; 
                cout << "Password: "; 
                cin >> p;
                
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
                            cout << "\n┌──────────────────────────────────────────┐\n";
                            cout << "│          SYSTEM ALERTS & NOTICES         │\n";
                            cout << "└──────────────────────────────────────────┘\n";
                            for (size_t j = 0; j < notes.size(); ++j) 
                            {
                                cout << " [!] " << notes[j] << "\n";
                            }
                            marketplace.clearNotifications(currentUser->getUserId());
                            printDivider();
                        }
                        break;
                    }
                }
                if (!currentUser) 
                {
                    cout << "\n>> ERROR: Authentication failed. Invalid credentials.\n";
                }
            }
        }
        else
        {
            printUserPanel(currentUser);
            cout << "  [1] Catalog New Asset (Book)\n";
            cout << "  [2] Browse Campus Inventory\n";
            cout << "  [3] Submit Resource Loan Request\n";
            cout << "  [4] Process Item Return & Log Evidence\n";
            cout << "  [5] Open System Inbox\n";
            cout << "  [6] Add Outgoing Pre-Loan Evidence\n";
            cout << "  [7] Modify Asset Trust Restrictions\n";
            cout << "  [8] Lodge Peer Damage Dispute\n";
            cout << "  [9] Administrative Arbitration Desk\n";
            cout << "  [10] Terminate Session (Logout)\n";
            printDivider();
            cout << "Select Operation: ";
            
            int choice; 
            cin >> choice; 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (choice == 10)
            {
                currentUser = nullptr;
                cout << "\n>> Session ended successfully.\n";
                continue;
            }
            if (choice == 1) 
            {
                printHeader("CATALOG NEW ASSET (BOOK)");
                string t, a, i; 
                int e;
                cout << "Book Title: "; getline(cin, t); 
                cout << "Author    : "; getline(cin, a); 
                cout << "ISBN Code : "; cin >> i; 
                cout << "Edition   : "; cin >> e;
                
                Book* b = new Book(currentUser->getUserId(), Resource::ImportanceLevel::MEDIUM, t, a, i, e);
                marketplace.addResource(b); 
                marketplace.saveResources("items.txt");
                
                printDivider();
                cout << ">> Asset successfully indexed!\n";
                cout << ">> Resource Tracking ID: " << b->getResourceId() << "\n";
            } 
            else if (choice == 2) 
            {
                printHeader("CAMPUS MARKETPLACE INVENTORY");
                const std::vector<Resource*>& res = marketplace.getResources();
                if (res.empty())
                {
                    cout << "   No entries found in the system registry.\n";
                }
                for (size_t i = 0; i < res.size(); ++i) 
                {
                    string s = (res[i]->getStatus() == Resource::Status::AVAILABLE) ? "AVAILABLE" : 
                               (res[i]->getStatus() == Resource::Status::OVERDUE ? "OVERDUE" : "LOANED");
                    
                    cout << "├─ ID: " << res[i]->getResourceId() << " | [" << s << "]\n";
                    cout << "│  Req Trust: " << res[i]->getMinTrustRequired() << " | Max Term: " << res[i]->getMaxLoanDuration() << " Days\n";
                    
                    if (res[i]->getResourceType() == "Book") 
                    { 
                        Book* b = dynamic_cast<Book*>(res[i]);
                        cout << "│  Item Type: Title - \"" << b->getTitle() << "\"\n"; 
                    }
                    printDivider();
                }
            } 
            else if (choice == 3) 
            {
                printHeader("SUBMIT RESOURCE LOAN REQUEST");
                int rid; 
                cout << "Enter target Resource Tracking ID: "; 
                cin >> rid;
                
                Resource* target = nullptr;
                const std::vector<Resource*>& res = marketplace.getResources();
                for (size_t i = 0; i < res.size(); ++i) 
                {
                    if (res[i]->getResourceId() == rid) 
                    {
                        target = res[i];
                    }
                }
                if (!target) 
                { 
                    cout << ">> ERROR: Action canceled. Target tracking ID not found.\n"; 
                    continue; 
                }
                
                string start = getCurrentDate();
                string due = getFutureDate(target->getMaxLoanDuration());
                
                printDivider();
                if (marketplace.requestLoan(currentUser, target, start, due))
                {
                    cout << ">> Loan transaction authorized successfully!\n";
                    cout << ">> Return Deadline assigned: " << due << "\n";
                }
                else
                {
                    cout << ">> TRANSACTION REJECTED: Resource unavailable or trust threshold unmet.\n";
                }
            } 
            else if (choice == 4) 
            {
                printHeader("PROCESS ITEM RETURN");
                int rid; 
                cout << "Enter returning Resource Tracking ID: "; 
                cin >> rid;
                
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
                    cout << ">> Base validation complete. Resource status updated.\n";
                    
                    string evPath;
                    cout << "\nEnter return condition file verification path (or type 'done' to skip): ";
                    while (getline(cin, evPath) && evPath != "done")
                    {
                        if (!evPath.empty())
                        {
                            active->addPostReturnEvidence(evPath);
                            cout << "   Evidence node attached. Add alternative path or type 'done': ";
                        }
                    }
                    
                    marketplace.saveResources("items.txt");
                    marketplace.saveTransactions("transactions.txt");
                    printDivider();
                    cout << ">> Return transaction closed completely.\n";
                } 
                else
                {
                    cout << ">> ERROR: Access denied. No matching active allocations found under your account.\n";
                }
            }
            else if (choice == 5)
            {
                printHeader("SYSTEM INBOX");
                auto notes = marketplace.getNotifications(currentUser->getUserId());
                if (notes.empty()) 
                {
                    cout << "   Your inbox is empty.\n";
                }
                else
                {
                    for (size_t i = 0; i < notes.size(); ++i) 
                    {
                        cout << " » " << notes[i] << "\n";
                    }
                    marketplace.clearNotifications(currentUser->getUserId());
                }
            }
            else if (choice == 6)
            {
                printHeader("ADD OUTGOING PRE-LOAN EVIDENCE");
                const std::vector<LoanTransaction*>& txs = marketplace.getTransactions();
                bool foundAny = false;
                for (size_t i = 0; i < txs.size(); ++i)
                {
                    if (txs[i]->getOwner()->getUserId() == currentUser->getUserId() && !txs[i]->isReturned())
                    {
                        cout << " -> Tx ID: " << txs[i]->getTransactionId() << " | Item ID: " << txs[i]->getResource()->getResourceId() << " | Borrower ID: " << txs[i]->getBorrower()->getUserId() << "\n";
                        foundAny = true;
                    }
                }
                if (!foundAny)
                {
                    cout << "   No active outgoing loans available for verification logging.\n";
                    continue;
                }
                
                int targetTxId;
                cout << "\nEnter target Transaction ID: ";
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
                    cout << "Enter pre-loan condition data validation link (or type 'done' to stop): ";
                    while (getline(cin, evPath) && evPath != "done")
                    {
                        if (!evPath.empty())
                        {
                            targetTx->addPreLoanEvidence(evPath);
                            cout << "   Evidence logged. Enter alternative entry path or type 'done': ";
                        }
                    }
                    marketplace.saveTransactions("transactions.txt");
                    cout << ">> Outgoing state documentation saved successfully.\n";
                }
                else
                {
                    cout << ">> ERROR: Access denied. Transaction ID invalid or entity validation mismatched.\n";
                }
            }
            else if (choice == 7) 
            {
                printHeader("MODIFY ASSET TRUST RESTRICTIONS");
                const std::vector<Resource*>& res = marketplace.getResources();
                bool ownsItems = false;
                for (size_t i = 0; i < res.size(); ++i)
                {
                    if (res[i]->getOwnerId() == currentUser->getUserId())
                    {
                        cout << " -> ID: " << res[i]->getResourceId() << " | Name: " << res[i]->getDisplayName() << "\n";
                        cout << "    [Current Profile] Trust Min: " << res[i]->getMinTrustRequired() << " | Allocation Cap: " << res[i]->getMaxLoanDuration() << " Days\n";
                        ownsItems = true;
                    }
                }
                
                if (!ownsItems)
                {
                    cout << "   You have not indexed any personal items in the repository.\n";
                    continue;
                }

                int targetId;
                cout << "\nEnter asset target ID to update: ";
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
                    cout << "Set Minimum User Trust Threshold: ";
                    cin >> newTrust;
                    cout << "Set Maximum Permitted Allocation Window (Days): ";
                    cin >> newDuration;

                    targetRes->setMinTrustRequired(newTrust);
                    targetRes->setMaxLoanDuration(newDuration);

                    marketplace.saveResources("items.txt");
                    printDivider();
                    cout << ">> System access configuration maps overwritten successfully.\n";
                }
                else
                {
                    cout << ">> ERROR: Configuration rejected. Resource mapping validation failed.\n";
                }
            }
            else if (choice == 8) 
            {
                printHeader("LODGE PEER DAMAGE DISPUTE");
                const std::vector<LoanTransaction*>& txs = marketplace.getTransactions();
                bool foundAny = false;
                for (size_t i = 0; i < txs.size(); ++i)
                {
                    if (txs[i]->getOwner()->getUserId() == currentUser->getUserId() && !txs[i]->isDisputed())
                    {
                        string statStr = (txs[i]->getStatus() == Transaction::Status::COMPLETED) ? "COMPLETED" : "ACTIVE";
                        cout << " -> Tx ID: " << txs[i]->getTransactionId() << " | Asset: " << txs[i]->getResource()->getDisplayName() << "\n";
                        cout << "    Borrower User ID: " << txs[i]->getBorrower()->getUserId() << " | Current State Flag: " << statStr << "\n";
                        foundAny = true;
                    }
                }

                if (!foundAny)
                {
                    cout << "   No historic records found eligible for litigation assignment.\n";
                    continue;
                }

                int targetTxId;
                cout << "\nEnter historical Transaction ID to freeze: ";
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
                    cout << "Provide statement itemizing damage parameters: ";
                    getline(cin, reason);

                    targetTx->raiseDispute(reason);
                    marketplace.saveTransactions("transactions.txt");
                    marketplace.saveUsers("users.txt");
                    printDivider();
                    cout << ">> Dispute flagged. Asset scores locked pending judicial execution review.\n";
                }
                else
                {
                    cout << ">> ERROR: Action aborted. Permission context verify failed.\n";
                }
            }
            else if (choice == 9) 
            {
                printHeader("ADMINISTRATIVE ARBITRATION DESK");
                const std::vector<LoanTransaction*>& txs = marketplace.getTransactions();
                bool foundAny = false;
                for (size_t i = 0; i < txs.size(); ++i)
                {
                    if (txs[i]->isDisputed() && !txs[i]->isDisputeResolved())
                    {
                        cout << " » Transaction ID Target: " << txs[i]->getTransactionId() << "\n";
                        cout << "   Asset Label: " << txs[i]->getResource()->getDisplayName() << "\n";
                        cout << "   Lender ID  : " << txs[i]->getOwner()->getUserId() << " | Borrower ID: " << txs[i]->getBorrower()->getUserId() << "\n";
                        cout << "   Statement  : \"" << txs[i]->getDisputeReason() << "\"\n";
                        
                        cout << "   [Audit Nodes] Handover Records: ";
                        const auto& pre = txs[i]->getPreLoanEvidence();
                        for (size_t k = 0; k < pre.size(); ++k) cout << pre[k] << " ";
                        cout << "\n   [Audit Nodes] Return Records  : ";
                        const auto& post = txs[i]->getPostReturnEvidence();
                        for (size_t k = 0; k < post.size(); ++k) cout << post[k] << " ";
                        cout << "\n" << string(40, '-') << "\n";
                        foundAny = true;
                    }
                }

                if (!foundAny)
                {
                    cout << "   No unresolved disputes pending active system arbitration.\n";
                    continue;
                }

                int targetTxId;
                cout << "Select Transaction ID to issue judicial settlement for: ";
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
                    cout << "\nEvaluate Evidence Ledger Verdict:\n  [1] Borrower at fault (Enforce Negligence Penalties)\n  [0] False Claim / Fraudulent Owner Statement\nSelect Judgment Node: ";
                    cin >> faultChoice;

                    targetTx->resolveDispute(faultChoice == 1);
                    marketplace.saveTransactions("transactions.txt");
                    marketplace.saveUsers("users.txt");
                    cout << ">> Case resolved. System state closed permanently. Trust arrays recalculated.\n";
                }
                else
                {
                    cout << ">> ERROR: Access denied or targeted case already adjudicated.\n";
                }
            }
        }
    }
    
    marketplace.saveUsers("users.txt");
    marketplace.saveResources("items.txt");
    marketplace.saveTransactions("transactions.txt");
    marketplace.saveNotifications("notifications.txt");
    cout << "\n>> System states flushed to disk safely. Goodbye!\n";
    return 0;
}