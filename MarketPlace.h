#ifndef MARKETPLACE_H
#define MARKETPLACE_H

#include <vector>
#include <string>

class User;
class Resource;
class LoanTransaction;

class Marketplace
{
    private:
        std::vector<User*> users;
        std::vector<Resource*> resources;
        std::vector<LoanTransaction*> transactions;

        int nextTransactionId;

        public:
        Marketplace();

        void addUser(User* user);
        void addResource(Resource* resource);

        LoanTransaction* requestLoan(User* borrower,Resource* resource,const std::string& startDate,const std::string& dueDate);

        // getters 
        const std::vector<User*>& getUsers() const;
        const std::vector<Resource*>& getResources() const;
        const std::vector<LoanTransaction*>& getTransactions() const;
    };

#endif
