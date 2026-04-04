#ifndef LABGEAR_H
#define LABGEAR_H

#include "Resource.h"
#include <string>

class LabGear : public Resource
{
private:
    std::string category;
    int safetyRating;
    bool requiresTraining;

public:
    // Constructor for new items (Auto-ID)
    LabGear(int ownerId,
            ImportanceLevel importance,
            const std::string &displayName,
            const std::string &category,
            int safetyRating,
            bool requiresTraining);

    // Constructor for loading existing items (Manual ID)
    LabGear(int resourceId,
            int ownerId,
            ImportanceLevel importance,
            const std::string &displayName,
            const std::string &category,
            int safetyRating,
            bool requiresTraining);

    // Getters
    std::string getCatergory() const;
    int getSafetyRating() const;
    bool needsTraining() const;

    std::string getResourceType() const override;
};

#endif