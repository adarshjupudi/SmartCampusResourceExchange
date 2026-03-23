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
    // owner is the resource owner
    User* owner = nullptr;

    for(User* u : users)
    {
        if(u->getUserId() == resource->getOwnerId())
        {
            owner = u;
            break;
        }
    }

    // owner not found -> invalid request
    if(owner == nullptr)
    {
        return nullptr;
    }

    // create transaction
    LoanTransaction* transaction =
        new LoanTransaction(nextTransactionId++,
                            borrower,
                            owner,
                            resource,
                            startDate,
                            dueDate);

    // process transaction
    bool success = transaction->process();

    // store transaction regardless (history matters)
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
    if(!file)
        return;   // file may not exist first time

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

        if(id > maxId)
            maxId = id;
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
        std::string status =
            (r->getStatus() == Resource::Status::AVAILABLE) ? "AVAILABLE" : "LOANED";

        if(r->getResourceType() == "Book")
        {
            Book* b = dynamic_cast<Book*>(r);

            file << "BOOK|"
                 << b->getResourceId() << "|"
                 << b->getOwnerId() << "|"
                 << b->getTitle() << "|"
                 << b->getAuthor() << "|"
                 << b->getIsbn() << "|"
                 << b->getEdition() << "|"
                 << status << "\n";
        }

        else if(r->getResourceType() == "Electronic")
        {
            Electronic* e = dynamic_cast<Electronic*>(r);

            file << "ELECTRONIC|"
                 << e->getResourceId() << "|"
                 << e->getOwnerId() << "|"
                 << e->getBrand() << "|"
                 << e->getModel() << "|"
                 << e->isWorking() << "|"
                 << e->hasBattery() << "|"
                 << status << "\n";
        }

        else if(r->getResourceType() == "LabGear")
        {
            LabGear* g = dynamic_cast<LabGear*>(r);

            file << "LABGEAR|"
                 << g->getResourceId() << "|"
                 << g->getOwnerId() << "|"
                 << g->getCatergory() << "|"
                 << g->getSafetyRating() << "|"
                 << g->needsTraining() << "|"
                 << status << "\n";
        }
    }
}
void Marketplace::saveTransactions(const std::string& filename)
{
    std::ofstream file(filename);

    for(LoanTransaction* t : transactions)
    {
        Resource* r = t->getResource();
        User* b = t->getBorrower();

        std::string status;

        if(t->getStatus() == Transaction::Status::PENDING)
            status = "PENDING";
        else if(t->getStatus() == Transaction::Status::ACTIVE)
            status = "ACTIVE";
        else if(t->getStatus() == Transaction::Status::COMPLETED)
            status = "COMPLETED";
        else
            status = "FAILED";

        file << "LOAN|"
             << t->getTransactionId() << "|"
             << b->getUserId() << "|"
             << r->getResourceId() << "|"
             << t->getStartDate() << "|"
             << t->getDueDate() << "|"
             << status
             << "\n";
    }
}
void Marketplace::loadTransactions(const std::string& filename)
{
    std::ifstream file(filename);
    if(!file) return;

    std::string line;

    while(std::getline(file, line))
    {
        std::stringstream ss(line);

        std::string type;
        std::getline(ss, type, '|');

        if(type == "LOAN")
        {
            std::string tidStr, borrowerStr, resourceStr, startDate, dueDate, statusStr;

            std::getline(ss, tidStr, '|');
            std::getline(ss, borrowerStr, '|');
            std::getline(ss, resourceStr, '|');
            std::getline(ss, startDate, '|');
            std::getline(ss, dueDate, '|');
            std::getline(ss, statusStr, '|');

            int tid = std::stoi(tidStr);
            int borrowerId = std::stoi(borrowerStr);
            int resourceId = std::stoi(resourceStr);

            User* borrower = nullptr;
            User* owner = nullptr;
            Resource* resource = nullptr;

            for(User* u : users)
            {
                if(u->getUserId() == borrowerId)
                    borrower = u;
            }

            for(Resource* r : resources)
            {
                if(r->getResourceId() == resourceId)
                {
                    resource = r;

                    for(User* u : users)
                        if(u->getUserId() == r->getOwnerId())
                            owner = u;
                }
            }

            if(borrower && owner && resource)
            {
                LoanTransaction* t =
                    new LoanTransaction(tid, borrower, owner, resource, startDate, dueDate);

                transactions.push_back(t);
            }
        }
    }
}
void Marketplace::loadResources(const std::string& filename)
{
    std::ifstream file(filename);
    if(!file) return;

    std::string line;

    while(std::getline(file, line))
    {
        std::stringstream ss(line);

        std::string type;
        std::getline(ss, type, '|');

        if(type == "BOOK")
        {
            std::string idStr, ownerStr, title, author, isbn, editionStr, statusStr;

            std::getline(ss, idStr, '|');
            std::getline(ss, ownerStr, '|');
            std::getline(ss, title, '|');
            std::getline(ss, author, '|');
            std::getline(ss, isbn, '|');
            std::getline(ss, editionStr, '|');
            std::getline(ss, statusStr, '|');

            int id = std::stoi(idStr);
            int ownerId = std::stoi(ownerStr);
            int edition = std::stoi(editionStr);

            Book* b = new Book(
                id,
                ownerId,
                Resource::ImportanceLevel::MEDIUM,
                title,
                author,
                isbn,
                edition
            );

            if(statusStr == "LOANED")
                b->setStatus(Resource::Status::LOANED);

            resources.push_back(b);
        }
    }
}