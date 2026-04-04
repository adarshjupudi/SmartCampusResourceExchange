#include "LabGear.h"

// Auto-ID version
LabGear::LabGear(int ownerId,
                 ImportanceLevel importance,
                 const std::string &displayName,
                 const std::string &category,
                 int safetyRating,
                 bool requiresTraining)
    : Resource(ownerId, importance, displayName)
{
    this->category = category;
    this->safetyRating = safetyRating;
    this->requiresTraining = requiresTraining;
}

// Manual ID version
LabGear::LabGear(int resourceId,
                 int ownerId,
                 ImportanceLevel importance,
                 const std::string &displayName,
                 const std::string &category,
                 int safetyRating,
                 bool requiresTraining)
    : Resource(resourceId, ownerId, importance, displayName)
{
    this->category = category;
    this->safetyRating = safetyRating;
    this->requiresTraining = requiresTraining;
}

std::string LabGear::getCatergory() const
{
    return category;
}

int LabGear::getSafetyRating() const
{
    return safetyRating;
}

bool LabGear::needsTraining() const
{
    return requiresTraining;
}

std::string LabGear::getResourceType() const
{
    return "LabGear";
}