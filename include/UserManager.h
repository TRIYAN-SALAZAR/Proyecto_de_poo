#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <vector>
#include "User.h"

class UserManager
{
private:
    std::vector<User> users;
public:
    UserManager();
    ~UserManager();

    void addUser(const User &u);
    User* findByName(const std::string &name);
    User* authenticate(const std::string &name, const std::string &password);
    std::vector<User>& allUsers();
    bool removeByName(const std::string &name);
};

#endif // USERMANAGER_H
