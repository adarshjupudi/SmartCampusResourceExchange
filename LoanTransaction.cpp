#include "LoanTransaction.h"
#include "User.h"
#include "Resource.h"

// constructor
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

// getters
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

bool LoanTransaction::isReturned() const
{
    return returned;
}

bool LoanTransaction::isLate() const
{
    if(!returned)
    {
        return false;
    }
    return returnDate > dueDate;
}
