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
        void processOption(int option);
        void run();
};

#endif // MENU_H
