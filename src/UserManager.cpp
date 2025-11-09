#include "UserManager.h"

UserManager::UserManager()
    : users()
{
}

UserManager::~UserManager() = default;

void UserManager::addUser(const User &u)
{
    // Insert a new user into the list. Using std::list means this
    // operation will not invalidate pointers to existing elements.
    users.push_back(u);
}

User* UserManager::findByName(const std::string &name)
{
    // Linear search across users. Returns a pointer to the element
    // inside the list. Because we use std::list, this pointer remains
    // valid until that specific element is erased.
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

std::list<User>& UserManager::allUsers()
{
    // Expose the internal list for iteration. Callers should prefer
    // read-only iteration; if they erase elements they must be aware
    // of iterator/pointer invalidation semantics for lists.
    return users;
}

bool UserManager::removeByName(const std::string &name)
{
    // Find the element and erase it. Erasing the element will invalidate
    // pointers/references to that specific User, but other element
    // pointers remain valid on std::list.
    for (auto it = users.begin(); it != users.end(); ++it) {
        if (it->getName() == name) {
            users.erase(it);
            return true;
        }
    }
    return false;
}
