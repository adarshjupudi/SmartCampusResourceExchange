#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

class Transaction
{

    public:

    enum class Status
    {
        PENDING,
        ACTIVE,
        COMPLETED,
        FAILED,
        DISPUTED
    };

    protected:

    int transactionId;
    Status status;
    std::string timestamp;

    public:

    Transaction(int transactionId);
    virtual ~Transaction(){}
    int getTransactionId() const;
    Status getStatus() const;
    std::string getTimestamp() const;

    virtual bool process()=0;
};
#endif