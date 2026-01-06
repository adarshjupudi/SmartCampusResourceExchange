#ifndef USER_H
#define USER_H

#include <string>
class User
{
    private:
    int userId;
    std::string name;
    std::string password;
    int trustPoints;
    public:
    //constructors
    User();
    User(int id,const std::string &name,const std::string &password);
    //getters
    int getUserId() const;
    std::string getName() const;
    int getTrustPoints() const;
    //login&authentication
    bool checkPassword(const std::string &password) const; 
    //trust logic(+ve/-ve)
    void updateTrust(int amount);
};

#endif