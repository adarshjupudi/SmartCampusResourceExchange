#include "Resource.h"

int Resource::nextId = 100; // Starting default for resource IDs

// Auto-ID Constructor for new items
Resource::Resource(int ownerId, ImportanceLevel importance, const std::string &displayName)
    : displayName(displayName),
      resourceId(nextId++),
      ownerId(ownerId),
      status(Status::AVAILABLE),
      importance(importance),
      minTrustRequired(0),
      maxLoanDuration(30)
{
}

// Manual ID Constructor for loading from files
Resource::Resource(int resourceId, int ownerId, ImportanceLevel importance, const std::string &displayName)
    : displayName(displayName),
      resourceId(resourceId),
      ownerId(ownerId),
      status(Status::AVAILABLE),
      importance(importance),
      minTrustRequired(0),
      maxLoanDuration(30)
{
}

void Resource::setNextId(int id) 
{ 
    nextId = id;
}

int Resource::getResourceId() const 
{ 
    return resourceId; 
}

int Resource::getOwnerId() const 
{ 
    return ownerId; 
}

Resource::Status Resource::getStatus() const 
{ 
    return status;
}

Resource::ImportanceLevel Resource::getImportance() const 
{ 
    return importance; 
}

std::string Resource::getDisplayName() const 
{ 
    return displayName; 
}

int Resource::getMinTrustRequired() const 
{ 
    return minTrustRequired;
}

int Resource::getMaxLoanDuration() const 
{ 
    return maxLoanDuration; 
}

void Resource::setMinTrustRequired(int trust)
{
    // Business rule protection: trust requirements cannot be negative
    if (trust < 0)
    {
        minTrustRequired = 0;
    }
    else
    {
        minTrustRequired = trust;
    }
}

void Resource::setMaxLoanDuration(int duration)
{
    // Business rule protection: loans must span at least 1 day and cannot exceed 365 days
    if (duration < 1)
    {
        maxLoanDuration = 1;
    }
    else if (duration > 365)
    {
        maxLoanDuration = 365;
    }
    else
    {
        maxLoanDuration = duration;
    }
}

void Resource::setStatus(Status newStatus) 
{ 
    status = newStatus;
}