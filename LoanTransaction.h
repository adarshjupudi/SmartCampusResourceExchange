#ifndef LOANTRANSACTION_H
#define LOANTRANSACTION_H

#include "Transaction.h"
#include <string>
#include <vector>

// forward declarations
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

    // condition evidence
    std::vector<std::string> preLoanEvidence;     // owner uploads
    std::vector<std::string> postReturnEvidence;  // borrower uploads

    bool returned;
    bool disputed;

    public:

    LoanTransaction(int transactionId,
                    User *borrower,
                    User *owner,
                    Resource *resource,
                    const std::string &startDate,
                    const std::string &dueDate);

    //transaction
    bool process() override;

    // return handling
    void markReturned(const std::string &returnDate);
    bool isReturned() const;
    bool isLate() const;

    // evidence handling
    void addPreLoanEvidence(const std::string &path);
    void addPostReturnEvidence(const std::string &path);

    // getters
    User *getBorrower() const;
    User *getOwner() const;
    Resource *getResource() const;
};

#endif
