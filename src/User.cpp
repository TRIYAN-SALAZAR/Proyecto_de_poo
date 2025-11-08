#include "User.h"
#include <iostream>
#include <limits>

// Simple incremental code generator for users
static int generateUserCode()
{
    static int counter = 1000;
    return ++counter;
}

User::User()
    : isSuperAdmin(false), isAdmin(false), isSeller(false), isWarehouseWorker(false),
      name(""), password(""), code(0)
{
}

User::~User() = default;

void User::getRol()
{
    std::cout << "Roles para usuario '" << name << "': ";
    bool any = false;
    if (isSuperAdmin) { std::cout << "SuperAdmin "; any = true; }
    if (isAdmin) { std::cout << "Admin "; any = true; }
    if (isSeller) { std::cout << "Seller "; any = true; }
    if (isWarehouseWorker) { std::cout << "WarehouseWorker "; any = true; }
    if (!any) std::cout << "(ninguno)";
    std::cout << std::endl;
}

int User::addNewUser(const User &actor)
{
    if (!(actor.isAdmin || actor.isSuperAdmin)) {
        std::cout << "Permisos insuficientes para agregar usuarios.\n";
        return 0;
    }

    int code = 0;
    std::cout << "Ingrese codigo: ";
    std::cin >> code;
    if (code) {
        std::cout << "Codigo vacio. Operacion cancelada." << std::endl;
        return 0;
    }

    std::cout << "Ingrese password: ";
    std::getline(std::cin, password);

    auto askRole = [](const char *prompt) -> bool {
        std::cout << prompt << " (y/n): ";
        char c = 'n';
        if (!(std::cin >> c)) return false;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return c == 'y' || c == 'Y';
    };

    isSuperAdmin = askRole("SuperAdmin?");
    isAdmin = askRole("Admin?");
    isSeller = askRole("Seller?");
    isWarehouseWorker = askRole("Warehouse worker?");

    code = generateUserCode();
    std::cout << "Usuario creado con codigo: " << code << std::endl;
    return code;
}

int User::deleteUser(const User &actor)
{
    if (!(actor.isAdmin || actor.isSuperAdmin)) {
        std::cout << "Permisos insuficientes para eliminar usuarios.\n";
        return 0;
    }

    std::cout << "Eliminar usuario '" << name << "'? (y/n): ";
    char c = 'n';
    if (!(std::cin >> c)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return 0;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (c == 'y' || c == 'Y') {
        isSuperAdmin = isAdmin = isSeller = isWarehouseWorker = false;
        name.clear();
        password.clear();
        code = 0;
        std::cout << "Usuario eliminado." << std::endl;
        return 1;
    }
    std::cout << "Operacion cancelada." << std::endl;
    return 0;
}