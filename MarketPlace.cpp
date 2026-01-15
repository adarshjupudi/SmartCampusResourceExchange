#include "Marketplace.h"
#include "User.h"
#include "Resource.h"
#include "LoanTransaction.h"

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
