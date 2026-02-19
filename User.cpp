#include "User.h"

int User::nextId = 1;

// Auto-ID constructor (used when registering new users)
User::User(const std::string& name, const std::string& password)
    : userId(nextId++), name(name), password(password), trustPoints(0)
{
}

// Explicit ID constructor (used when loading from file)
User::User(int id, const std::string& name, const std::string& password)
    : userId(id), name(name), password(password), trustPoints(0)
{
}

int User::getUserId() const
{
    return userId;
}

std::string User::getName() const
{
    return name;
}

std::string User::getPassword() const
{
    return password;
}

int User::getTrustPoints() const
{
    return trustPoints;
}

bool User::checkPassword(const std::string& password) const
{
    return this->password == password;
}

void User::updateTrust(int amount)
{
    trustPoints += amount;
}

void User::setNextId(int id)
{
    nextId = id;
}