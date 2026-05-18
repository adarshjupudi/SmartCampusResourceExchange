#include "LoanTransaction.h"
#include "Resource.h"
#include "User.h"

LoanTransaction::LoanTransaction(int transactionId,
                                 User *borrower,
                                 User *owner,
                                 Resource *resource,
                                 const std::string &startDate,
                                 const std::string &dueDate)
    : Transaction(transactionId),
      borrower(borrower),
      owner(owner),
      resource(resource),
      startDate(startDate),
      dueDate(dueDate),
      returnDate(""),
      returned(false),
      disputed(false)
{
    status = Status::PENDING;
}

User *LoanTransaction::getBorrower() const
{
    return borrower;
}

User *LoanTransaction::getOwner() const
{
    return owner;
}

Resource *LoanTransaction::getResource() const
{
    return resource;
}

std::string LoanTransaction::getStartDate() const
{
    return startDate;
}

std::string LoanTransaction::getDueDate() const
{
    return dueDate;
}

bool LoanTransaction::isReturned() const
{
    return returned;
}

bool LoanTransaction::isLate() const
{
    if (!returned)
    {
        return false;
    }
    return returnDate > dueDate;
}

bool LoanTransaction::process()
{
    if (status != Status::PENDING)
    {
        return false;
    }
    if (resource->getStatus() != Resource::Status::AVAILABLE)
    {
        status = Status::FAILED;
        return false;
    }
    if (borrower->getTrustPoints() < resource->getMinTrustRequired())
    {
        status = Status::FAILED;
        return false;
    }
    resource->setStatus(Resource::Status::LOANED);
    status = Status::ACTIVE;
    return true;
}

void LoanTransaction::markReturned(const std::string &date)
{
    if (status != Status::ACTIVE)
    {
        return;
    }
    returnDate = date;
    returned = true;
    resource->setStatus(Resource::Status::AVAILABLE);
    status = Status::COMPLETED;
    
    if (isLate())
    {
        borrower->updateTrust(-10);
        owner->updateTrust(+6);
    }
    else
    {
        borrower->updateTrust(+2);
        owner->updateTrust(+5);
    }
}

void LoanTransaction::addPreLoanEvidence(const std::string &path)
{
    preLoanEvidence.push_back(path);
}

void LoanTransaction::addPostReturnEvidence(const std::string &path)
{
    postReturnEvidence.push_back(path);
}

const std::vector<std::string>& LoanTransaction::getPreLoanEvidence() const
{
    return preLoanEvidence;
}

const std::vector<std::string>& LoanTransaction::getPostReturnEvidence() const
{
    return postReturnEvidence;
}