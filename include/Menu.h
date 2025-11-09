#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>
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
        void printUserBanner() const;
        // Interactive selector helper (uses arrow keys + Enter on Windows).
        // Returns selected index (0-based) or -1 on Esc/cancel.
        int selectFromList(const std::vector<std::string> &items, const std::string &title = "", int startIndex = 0, bool allowEsc = true);
        // Build and display the main menu as an interactive selector. Returns
        // the mapped option number (same numbers used by processOption) or
        // -1 to signal exit/cancel.
        int interactiveSelectMainMenu();
    
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
