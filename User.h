#ifndef USER_H
#define USER_H

#include <string>

class User
{
private:
    static int nextId;         
    int userId;
    std::string name;
    std::string password;
    int trustPoints;

public:
    // Constructors
    User(const std::string& name, const std::string& password); // auto-ID
    User(int id, const std::string& name, const std::string& password); // for loading

    // Getters
    int getUserId() const;
    std::string getName() const;
    std::string getPassword() const;
    int getTrustPoints() const;

    // Authentication
    bool checkPassword(const std::string& password) const;

    // Trust logic
    void updateTrust(int amount);

    // Static ID control (for persistence resume)
    static void setNextId(int id);
};

#endif
