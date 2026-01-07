#ifndef LABGEAR_H
#define LABGEAR_H

#include "Resource.h"
#include <string>

class LabGear:public Resource
{
    private:
    std::string category;
    int safetyRating;
    bool requiresTraining;
    public:
    LabGear(int resourceId,
            int ownerId,
            ImportanceLevel importance,
            const std::string &displayName,
            const std::string &category,
            int safetyRating,
            bool requiresTraining
            );

    //getters
    std::string getCatergory() const;
    int getSafetyRating() const;
    bool needsTraining() const;

    std::string getResourceType() const override;
};

#endif