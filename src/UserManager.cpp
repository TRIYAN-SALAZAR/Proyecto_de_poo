#include "UserManager.h"

UserManager::UserManager()
    : users()
{
}

UserManager::~UserManager() = default;

void UserManager::addUser(const User &u)
{
    users.push_back(u);
}

User* UserManager::findByName(const std::string &name)
{
    for (auto &u : users) {
        if (u.getName() == name) return &u;
    }
    return nullptr;
}

User* UserManager::authenticate(const std::string &name, const std::string &password)
{
    User* u = findByName(name);
    if (!u) return nullptr;
    if (u->checkPassword(password)) return u;
    return nullptr;
}

std::vector<User>& UserManager::allUsers()
{
    return users;
}
