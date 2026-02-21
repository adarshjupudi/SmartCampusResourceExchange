#include "Resource.h"

Resource::Resource(int resourceId,
                   int ownerId,
                   ImportanceLevel importance,
                   const std::string &displayName)
    : displayName(displayName),
      resourceId(resourceId),
      ownerId(ownerId),
      status(Status::AVAILABLE),
      importance(importance),
      minTrustRequired(0),
      maxLoanDuration(30)
{
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

void Resource::setStatus(Status newStatus)
{
    status = newStatus;
}