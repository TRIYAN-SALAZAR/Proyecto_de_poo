#ifndef MENU_H
#define MENU_H

#include <string>
#include <Gestor.h>
#include "UserManager.h"
#include "User.h"

class Menu
{
    private:
        Gestor* gestor;
        UserManager* userManager;
        User* currentUser;
    
    public:
    Menu(Gestor* gestor, UserManager* userManager);

    void showMainMenu();
        void showProductTypeMenu() const;
        void showOperatorsMenu() const;
        void processOption(int option);
        void run();

        void operatorsMenu();
        void modifyProduct();
};

#endif // MENU_H
