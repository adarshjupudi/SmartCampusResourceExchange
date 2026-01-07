#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

class Resource
{
    public:

    //life cycle of a product/resource
    enum class Status
    {
        AVAILABLE,
        LOANED,
        OVERDUE,
    };
    //Importance-Level associated with a resource
    enum class ImportanceLevel
    {
        LOW,
        MEDIUM,
        HIGH,
    };

    protected:

    std::string displayName;
    int resourceId;
    int ownerId;
    Status status;
    ImportanceLevel importance;
    //owner controlled rules
    int minTrustRequired;
    int maxLoanDuration;

    public:

    //constructor
    Resource(int resourceId,int ownerId,ImportanceLevel importance,const std::string &displayName);
    virtual ~Resource(){};

    //getters
    int getResourceId() const;
    int getOwnerId() const;
    Status getStatus() const;
    ImportanceLevel getImportance() const;
    std::string getDisplayName() const;

    //owner rules
    int getMinTrustRequired() const;
    int getMaxLoanDuration() const;

    //status change
    void setStatus(Status newStatus);

    virtual std::string getResourceType() const = 0;
};

#endif