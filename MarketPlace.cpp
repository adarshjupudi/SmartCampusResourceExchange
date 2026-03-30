#include "Marketplace.h"
#include "User.h"
#include "Resource.h"
#include "LoanTransaction.h"
#include "Book.h"
#include "Electronic.h"
#include "LabGear.h"
#include <fstream>
#include <sstream>

Marketplace::Marketplace()
    : nextTransactionId(1)
{
}

// register a user
void Marketplace::addUser(User* user)
{
    users.push_back(user);
}

// register a resource
void Marketplace::addResource(Resource* resource)
{
    resources.push_back(resource);
}

// request a loan
LoanTransaction* Marketplace::requestLoan(User* borrower,
                                          Resource* resource,
                                          const std::string& startDate,
                                          const std::string& dueDate)
{
    User* owner = nullptr;

    for(User* u : users)
    {
        if(u->getUserId() == resource->getOwnerId())
        {
            owner = u;
            break;
        }
    }

    if(owner == nullptr)
    {
        return nullptr;
    }

    LoanTransaction* transaction =
        new LoanTransaction(nextTransactionId++,
                            borrower,
                            owner,
                            resource,
                            startDate,
                            dueDate);

    bool success = transaction->process();
    transactions.push_back(transaction);

    if(!success)
    {
        return nullptr;
    }

    return transaction;
}

// getters
const std::vector<User*>& Marketplace::getUsers() const
{
    return users;
}

const std::vector<Resource*>& Marketplace::getResources() const
{
    return resources;
}

const std::vector<LoanTransaction*>& Marketplace::getTransactions() const
{
    return transactions;
}

void Marketplace::loadUsers(const std::string& filename)
{
    std::ifstream file(filename);
    if(!file) return;

    std::string line;
    int maxId = 0;

    while(std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string idStr, name, password, trustStr;

        std::getline(ss, idStr, '|');
        std::getline(ss, name, '|');
        std::getline(ss, password, '|');
        std::getline(ss, trustStr, '|');

        int id = std::stoi(idStr);
        int trust = std::stoi(trustStr);

        User* user = new User(id, name, password);
        user->updateTrust(trust);
        users.push_back(user);

        if(id > maxId) maxId = id;
    }
    User::setNextId(maxId + 1);
}

void Marketplace::saveUsers(const std::string& filename)
{
    std::ofstream file(filename);
    for(User* user : users)
    {
        file << user->getUserId() << "|"
             << user->getName() << "|"
             << user->getPassword() << "|"
             << user->getTrustPoints() << "\n";
    }
}

void Marketplace::saveResources(const std::string& filename)
{
    std::ofstream file(filename);
    for(Resource* r : resources)
    {
        std::string status = (r->getStatus() == Resource::Status::AVAILABLE) ? "AVAILABLE" : "LOANED";

        if(r->getResourceType() == "Book")
        {
            Book* b = dynamic_cast<Book*>(r);
            file << "BOOK|" << b->getResourceId() << "|" << b->getOwnerId() << "|"
                 << b->getTitle() << "|" << b->getAuthor() << "|" << b->getIsbn() << "|"
                 << b->getEdition() << "|" << status << "\n";
        }
        else if(r->getResourceType() == "Electronic")
        {
            Electronic* e = dynamic_cast<Electronic*>(r);
            file << "ELECTRONIC|" << e->getResourceId() << "|" << e->getOwnerId() << "|"
                 << e->getBrand() << "|" << e->getModel() << "|" << e->isWorking() << "|"
                 << e->hasBattery() << "|" << status << "\n";
        }
        else if(r->getResourceType() == "LabGear")
        {
            LabGear* g = dynamic_cast<LabGear*>(r);
            file << "LABGEAR|" << g->getResourceId() << "|" << g->getOwnerId() << "|"
                 << g->getCatergory() << "|" << g->getSafetyRating() << "|"
                 << g->needsTraining() << "|" << status << "\n";
        }
    }
}

void Marketplace::loadResources(const std::string& filename)
{
    std::ifstream file(filename);
    if(!file) return;

    std::string line;
    int maxId = 0;

    while(std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string type;
        std::getline(ss, type, '|');

        if(type == "BOOK")
        {
            std::string idStr, ownerStr, title, author, isbn, edStr, statusStr;
            std::getline(ss, idStr, '|');
            std::getline(ss, ownerStr, '|');
            std::getline(ss, title, '|');
            std::getline(ss, author, '|');
            std::getline(ss, isbn, '|');
            std::getline(ss, edStr, '|');
            std::getline(ss, statusStr, '|');

            int id = std::stoi(idStr);
            if(id > maxId) maxId = id;

            Book* b = new Book(id, std::stoi(ownerStr), Resource::ImportanceLevel::MEDIUM, 
                               title, author, isbn, std::stoi(edStr));
            if(statusStr == "LOANED") b->setStatus(Resource::Status::LOANED);
            resources.push_back(b);
        }
        else if(type == "ELECTRONIC")
        {
            std::string idStr, ownerStr, brand, model, workStr, battStr, statusStr;
            std::getline(ss, idStr, '|');
            std::getline(ss, ownerStr, '|');
            std::getline(ss, brand, '|');
            std::getline(ss, model, '|');
            std::getline(ss, workStr, '|');
            std::getline(ss, battStr, '|');
            std::getline(ss, statusStr, '|');

            int id = std::stoi(idStr);
            if(id > maxId) maxId = id;

            Electronic* e = new Electronic(id, std::stoi(ownerStr), Resource::ImportanceLevel::MEDIUM,
                                           brand, brand, model, std::stoi(workStr), std::stoi(battStr));
            if(statusStr == "LOANED") e->setStatus(Resource::Status::LOANED);
            resources.push_back(e);
        }
        else if(type == "LABGEAR")
        {
            std::string idStr, ownerStr, cat, safeStr, trainStr, statusStr;
            std::getline(ss, idStr, '|');
            std::getline(ss, ownerStr, '|');
            std::getline(ss, cat, '|');
            std::getline(ss, safeStr, '|');
            std::getline(ss, trainStr, '|');
            std::getline(ss, statusStr, '|');

            int id = std::stoi(idStr);
            if(id > maxId) maxId = id;

            LabGear* g = new LabGear(id, std::stoi(ownerStr), Resource::ImportanceLevel::MEDIUM,
                                     cat, cat, std::stoi(safeStr), std::stoi(trainStr));
            if(statusStr == "LOANED") g->setStatus(Resource::Status::LOANED);
            resources.push_back(g);
        }
    }
    Resource::setNextId(maxId + 1);
}

void Marketplace::saveTransactions(const std::string& filename)
{
    std::ofstream file(filename);
    for(LoanTransaction* t : transactions)
    {
        std::string status;
        if(t->getStatus() == Transaction::Status::PENDING) status = "PENDING";
        else if(t->getStatus() == Transaction::Status::ACTIVE) status = "ACTIVE";
        else if(t->getStatus() == Transaction::Status::COMPLETED) status = "COMPLETED";
        else status = "FAILED";

        file << "LOAN|" << t->getTransactionId() << "|" << t->getBorrower()->getUserId() << "|"
             << t->getResource()->getResourceId() << "|" << t->getStartDate() << "|"
             << t->getDueDate() << "|" << status << "\n";
    }
}

void Marketplace::loadTransactions(const std::string& filename)
{
    std::ifstream file(filename);
    if(!file) return;

    std::string line;
    int maxTid = 0;

    while(std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string type;
        std::getline(ss, type, '|');

        if(type == "LOAN")
        {
            std::string tidS, bIdS, rIdS, start, due, status;
            std::getline(ss, tidS, '|');
            std::getline(ss, bIdS, '|');
            std::getline(ss, rIdS, '|');
            std::getline(ss, start, '|');
            std::getline(ss, due, '|');
            std::getline(ss, status, '|');

            int tid = std::stoi(tidS);
            if(tid > maxTid) maxTid = tid;

            User* borrower = nullptr;
            User* owner = nullptr;
            Resource* res = nullptr;

            for(User* u : users) if(u->getUserId() == std::stoi(bIdS)) borrower = u;
            for(Resource* r : resources) {
                if(r->getResourceId() == std::stoi(rIdS)) {
                    res = r;
                    for(User* u : users) if(u->getUserId() == r->getOwnerId()) owner = u;
                }
            }

            if(borrower && owner && res) {
                LoanTransaction* t = new LoanTransaction(tid, borrower, owner, res, start, due);
                transactions.push_back(t);
            }
        }
    }
    nextTransactionId = maxTid + 1;
}