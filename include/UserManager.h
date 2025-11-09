#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <list>
#include "User.h"

class UserManager
{
private:
    // Use std::list to ensure that pointers/references to stored User
    // objects remain valid across insertions and deletions (splice/erase
    // rules apply). This avoids dangling pointers for code that stores
    // User* to elements (for example Menu::currentUser). A vector would
    // reallocate on push_back and invalidate pointers.
    std::list<User> users;
public:
    UserManager();
    ~UserManager();

    void addUser(const User &u);
    User* findByName(const std::string &name);
    User* authenticate(const std::string &name, const std::string &password);
    // Return a reference to the internal container. Callers can iterate
    // over the list with range-for. We expose the container here for
    // simplicity (caller must not invalidate iterators by erasing the
    // element they're holding a pointer to).
    std::list<User>& allUsers();
    bool removeByName(const std::string &name);
};

#endif // USERMANAGER_H
