#include "Resource.h"

int Resource::nextId = 100; // Starting default for resource IDs

// Auto-ID Constructor for new items
Resource::Resource(int ownerId, ImportanceLevel importance, const std::string &displayName)
    : displayName(displayName),
      resourceId(nextId++), // Assigns current nextId and increments it
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
      resourceId(resourceId), // Uses provided ID
      ownerId(ownerId),
      status(Status::AVAILABLE),
      importance(importance),
      minTrustRequired(0),
      maxLoanDuration(30)
{
}

void Resource::setNextId(int id) { nextId = id; }

int Resource::getResourceId() const { return resourceId; }
int Resource::getOwnerId() const { return ownerId; }
Resource::Status Resource::getStatus() const { return status; }
Resource::ImportanceLevel Resource::getImportance() const { return importance; }
std::string Resource::getDisplayName() const { return displayName; }
int Resource::getMinTrustRequired() const { return minTrustRequired; }
int Resource::getMaxLoanDuration() const { return maxLoanDuration; }

void Resource::setStatus(Status newStatus) { status = newStatus; }