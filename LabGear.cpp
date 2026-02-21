#include "LabGear.h"

LabGear::LabGear(int resourceId,
                 int ownerId,
                 ImportanceLevel importance,
                 const std::string &displayName,
                 const std::string &category,
                 int safetyRating,
                 bool requiresTraining)
    : Resource(resourceId, ownerId, importance, displayName),
      category(category),
      safetyRating(safetyRating),
      requiresTraining(requiresTraining)
{
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