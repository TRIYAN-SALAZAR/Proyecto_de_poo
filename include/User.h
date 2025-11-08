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

        // Accessors used by authentication and menu
        const string& getName() const;
        int getCode() const;
        bool isAdminRole() const;
        bool isSuperAdminRole() const;
        bool isSellerRole() const;
        bool isWarehouseWorkerRole() const;

        // Simple password check (plain-text for now)
        bool checkPassword(const string &pw) const;

        // Setters for roles (used by superadmin)
        void setAdmin(bool v);
        void setSuperAdmin(bool v);
        void setSeller(bool v);
        void setWarehouseWorker(bool v);
        void setPassword(const string &pw);
    void setName(const string &n);

};

#endif // USER_H
