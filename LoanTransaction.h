#ifndef LOANTRANSACTION_H
#define LOANTRANSACTION_H

#include "Transaction.h"
#include <string>
#include <vector>

class User;
class Resource;

class LoanTransaction : public Transaction
{
private:
    User *borrower;
    User *owner;
    Resource *resource;

    std::string startDate;
    std::string dueDate;
    std::string returnDate;

    // Condition evidence lists
    std::vector<std::string> preLoanEvidence;     // Owner uploads
    std::vector<std::string> postReturnEvidence;  // Borrower uploads

    bool returned;
    bool disputed;

public:
    LoanTransaction(int transactionId,
                    User *borrower,
                    User *owner,
                    Resource *resource,
                    const std::string &startDate,
                    const std::string &dueDate);

    // Core transaction operations
    bool process() override;

    // Return handling
    void markReturned(const std::string &returnDate);
    bool isReturned() const;
    bool isLate() const;

    // Evidence modification methods
    void addPreLoanEvidence(const std::string &path);
    void addPostReturnEvidence(const std::string &path);

    // Getters
    User *getBorrower() const;
    User *getOwner() const;
    Resource *getResource() const;

    std::string getStartDate() const;
    std::string getDueDate() const;
    
    // Constant reference getters for data serialization
    const std::vector<std::string>& getPreLoanEvidence() const;
    const std::vector<std::string>& getPostReturnEvidence() const;
};

#endif