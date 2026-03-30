#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

class Resource
{
public:
    enum class Status { AVAILABLE, LOANED, OVERDUE };
    enum class ImportanceLevel { LOW, MEDIUM, HIGH };

protected:
    std::string displayName;
    int resourceId;
    int ownerId;
    Status status;
    ImportanceLevel importance;
    int minTrustRequired;
    int maxLoanDuration;

private:
    static int nextId; // Added to track the next available ID internally

public:
    // Constructor 1: For new resources (Auto-ID)
    Resource(int ownerId, ImportanceLevel importance, const std::string &displayName);
    
    // Constructor 2: For loading existing resources (Manual ID)
    Resource(int resourceId, int ownerId, ImportanceLevel importance, const std::string &displayName);
    
    static void setNextId(int id);
    virtual ~Resource(){};

    // Getters
    int getResourceId() const;
    int getOwnerId() const;
    Status getStatus() const;
    ImportanceLevel getImportance() const;
    std::string getDisplayName() const;
    int getMinTrustRequired() const;
    int getMaxLoanDuration() const;

    // Status management
    void setStatus(Status newStatus);
    virtual std::string getResourceType() const = 0;
};

#endif