#include "Electronic.h"

Electronic::Electronic(int resourceId,
                       int ownerId,
                       ImportanceLevel importance,
                       const std::string &displayName,
                       const std::string &brand,
                       const std::string &model,
                       bool workingCondition,
                       bool batteryIncluded)
    : Resource(resourceId, ownerId, importance, displayName),
      brand(brand),
      model(model),
      workingCondition(workingCondition),
      batteryIncluded(batteryIncluded)
{
}

std::string Electronic::getBrand() const { return brand; }
std::string Electronic::getModel() const { return model; }
bool Electronic::isWorking() const { return workingCondition; }
bool Electronic::hasBattery() const { return batteryIncluded; }

std::string Electronic::getResourceType() const
{
    return "Electronic";
}