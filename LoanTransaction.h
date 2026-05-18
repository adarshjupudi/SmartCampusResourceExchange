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

    std::vector<std::string> preLoanEvidence;     
    std::vector<std::string> postReturnEvidence;  

    bool returned;
    bool disputed;
    std::string disputeReason;
    bool disputeResolved;

public:
    LoanTransaction(int transactionId,
                    User *borrower,
                    User *owner,
                    Resource *resource,
                    const std::string &startDate,
                    const std::string &dueDate);

    bool process() override;

    void markReturned(const std::string &returnDate);
    bool isReturned() const;
    bool isLate() const;

    void addPreLoanEvidence(const std::string &path);
    void addPostReturnEvidence(const std::string &path);

    User *getBorrower() const;
    User *getOwner() const;
    Resource *getResource() const;

    std::string getStartDate() const;
    std::string getDueDate() const;
    
    const std::vector<std::string>& getPreLoanEvidence() const;
    const std::vector<std::string>& getPostReturnEvidence() const;

    // Dispute Lifecycle Operations
    void raiseDispute(const std::string &reason);
    void resolveDispute(bool borrowerAtFault);
    
    bool isDisputed() const;
    bool isDisputeResolved() const;
    std::string getDisputeReason() const;

    // Loading helper for database reconstruction
    void loadDisputeState(bool isDisp, const std::string &reason, bool isResolved);
};

#endif