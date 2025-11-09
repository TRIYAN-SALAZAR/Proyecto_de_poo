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
    // Consume leftover newline if any
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string uname;
    std::cout << "Ingrese nombre de usuario: ";
    std::getline(std::cin, uname);
    if (uname.empty()) {
        std::cout << "Nombre vacio. Operacion cancelada." << std::endl;
        return 0;
    }

    std::string pw;
    std::cout << "Ingrese password: ";
    std::getline(std::cin, pw);
    if (pw.empty()) {
        std::cout << "Password vacio. Operacion cancelada." << std::endl;
        return 0;
    }

    auto askRole = [](const char *prompt) -> bool {
        std::cout << prompt << " (y/n): ";
        char c = 'n';
        if (!(std::cin >> c)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return false; }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return c == 'y' || c == 'Y';
    };

    bool s = false;
    bool a = false;
    // Only SuperAdmin actors can assign SuperAdmin or Admin roles when creating users
    if (actor.isSuperAdmin) {
        s = askRole("SuperAdmin?");
        a = askRole("Admin?");
    } else {
        // actor is at least Admin (because permission check passed) but not SuperAdmin
        std::cout << "(Aviso) Como Admin no puede asignar roles Admin ni SuperAdmin al crear usuarios.\n";
    }
    bool sel = askRole("Seller?");
    bool w = askRole("Warehouse worker?");

    // Confirm
    std::cout << "Confirma crear usuario '" << uname << "' con los roles: ";
    if (s) std::cout << "SuperAdmin ";
    if (a) std::cout << "Admin ";
    if (sel) std::cout << "Seller ";
    if (w) std::cout << "WarehouseWorker ";
    if (!s && !a && !sel && !w) std::cout << "(ninguno)";
    std::cout << " ? (y/n): ";
    char conf = 'n';
    if (!(std::cin >> conf)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::cout << "Operacion cancelada." << std::endl; return 0; }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (!(conf == 'y' || conf == 'Y')) { std::cout << "Operacion cancelada." << std::endl; return 0; }

    // apply values to this user object
    name = uname;
    password = pw;
    isSuperAdmin = s;
    isAdmin = a;
    isSeller = sel;
    isWarehouseWorker = w;

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

// Accessors
const string& User::getName() const { return name; }
int User::getCode() const { return code; }
bool User::isAdminRole() const { return isAdmin; }
bool User::isSuperAdminRole() const { return isSuperAdmin; }
bool User::isSellerRole() const { return isSeller; }
bool User::isWarehouseWorkerRole() const { return isWarehouseWorker; }

bool User::checkPassword(const string &pw) const { return password == pw; }

void User::setAdmin(bool v) { isAdmin = v; }
void User::setSuperAdmin(bool v) { isSuperAdmin = v; }
void User::setSeller(bool v) { isSeller = v; }
void User::setWarehouseWorker(bool v) { isWarehouseWorker = v; }
void User::setPassword(const string &pw) { password = pw; }
void User::setName(const string &n) { name = n; }