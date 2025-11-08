#ifndef USER_H
#define USER_H

#include <string>

using std::string;

class User
{
    private:
        bool isSuperAdmin;
        bool isAdmin;
        bool isSeller;
        bool isWarehouseWorker;
        
        string name;
        string password;
        int code;
        
    public:
        User();
        virtual ~User();

    void getRol();
    int addNewUser(const User &actor);
    int deleteUser(const User &actor);

};

#endif // USER_H
