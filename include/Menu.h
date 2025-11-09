#ifndef MENU_H
#define MENU_H

#include <string>
#include <Gestor.h>
#include "UserManager.h"
#include "User.h"
#include "SalesManager.h"

class Menu
{
    private:
        Gestor* gestor;
        UserManager* userManager;
        User* currentUser;
        SalesManager* salesManager;
    
    public:
    Menu(Gestor* gestor, UserManager* userManager, SalesManager* salesManager);

    void showMainMenu();
    void showProductTypeMenu() const;
    void showOperatorsMenu() const;
    bool processOption(int option);
    void run();

        void operatorsMenu();
        void modifyProduct();
        void salesMenu();
};

#endif // MENU_H
