#ifndef MENU_H
#define MENU_H

#include <string>
#include <Gestor.h>

class Menu
{
    private:
        Gestor* gestor;
    
    public:
        Menu(Gestor* gestor);

        void showMainMenu() const;
        void showProductTypeMenu() const;
    void showOperatorsMenu() const;
        void processOption(int option);
        void run();

    void operatorsMenu();
};

#endif // MENU_H
