#include "Transaction.h"

Transaction::Transaction(int transactionId)
: transactionId(transactionId),status(Status::PENDING),timestamp("")
{
    
}

int Transaction::getTransactionId() const
{
    return transactionId;
}

Transaction::Status Transaction::getStatus() const
{
    return status;
}

std::string Transaction::getTimestamp() const
{
    return timestamp;
}