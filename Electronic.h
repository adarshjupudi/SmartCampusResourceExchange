#ifndef ELECTRONIC_H
#define ELECTRONIC_H

#include "Resource.h"
#include <string>

class Electronic:public Resource
{
    private:
    std::string brand;
    std::string model;
    bool workingCondition;
    bool batteryIncluded;
    public:
    Electronic(int resourceId,
               int ownerId,
               ImportanceLevel importance,
               const std::string &displayName,
               const std::string & brand,
               const std::string & model,
               bool workingCondition,
               bool batteryIncluded
               );
    
    //getters
    std::string getBrand() const;
    std::string getModel() const;
    bool isWorking() const;
    bool hasBattery() const;

    std::string getResourceType() const override;

};
#endif