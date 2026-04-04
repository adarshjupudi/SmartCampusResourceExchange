#ifndef MARKETPLACE_H
#define MARKETPLACE_H

#include <vector>
#include <string>
#include <map>

class User;
class Resource;
class LoanTransaction;

class Marketplace
{
private:
    std::vector<User*> users;
    std::vector<Resource*> resources;
    std::vector<LoanTransaction*> transactions;
    
    // Map to store notifications: Key is userId, Value is a list of messages
    std::map<int, std::vector<std::string>> notifications;

    int nextTransactionId;

public:
    Marketplace();

    void addUser(User* user);
    void addResource(Resource* resource);

    LoanTransaction* requestLoan(User* borrower,
                                 Resource* resource,
                                 const std::string& startDate,
                                 const std::string& dueDate);

    void updateOverdueStatus(const std::string& currentDate);

    // Notification Logic
    void addNotification(int userId, const std::string& message);
    std::vector<std::string> getNotifications(int userId);
    void clearNotifications(int userId);

    const std::vector<User*>& getUsers() const;
    const std::vector<Resource*>& getResources() const;
    const std::vector<LoanTransaction*>& getTransactions() const;

    void loadUsers(const std::string& filename);
    void saveUsers(const std::string& filename);

    void loadResources(const std::string& filename);
    void saveResources(const std::string& filename);

    void loadTransactions(const std::string& filename);
    void saveTransactions(const std::string& filename);

    // Persistence for notifications
    void loadNotifications(const std::string& filename);
    void saveNotifications(const std::string& filename);
};

#endif