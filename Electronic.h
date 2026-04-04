#ifndef ELECTRONIC_H
#define ELECTRONIC_H

#include "Resource.h"
#include <string>

class Electronic : public Resource
{
private:
    std::string brand;
    std::string model;
    bool workingCondition;
    bool batteryIncluded;

public:
    // Constructor for new items (Auto-ID)
    Electronic(int ownerId,
               ImportanceLevel importance,
               const std::string &displayName,
               const std::string &brand,
               const std::string &model,
               bool workingCondition,
               bool batteryIncluded);

    // Constructor for loading existing items (Manual ID)
    Electronic(int resourceId,
               int ownerId,
               ImportanceLevel importance,
               const std::string &displayName,
               const std::string &brand,
               const std::string &model,
               bool workingCondition,
               bool batteryIncluded);

    // Getters
    std::string getBrand() const;
    std::string getModel() const;
    bool isWorking() const;
    bool hasBattery() const;

    std::string getResourceType() const override;
};

#endif