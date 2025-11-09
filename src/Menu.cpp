#include <iostream>
#include <limits>
#include "Menu.h"
#include "Electronic.h"
#include "Book.h"
#include "UTILITIES.h"
#include <iomanip>
#include <sstream>

Menu::Menu(Gestor *gestor, UserManager* userManager, SalesManager* salesManager)
    : gestor(gestor), userManager(userManager), currentUser(nullptr), salesManager(salesManager)
{
}

void Menu::printUserBanner() const
{
    if (!currentUser) return;
    std::ostringstream oss;
    oss << "User: " << currentUser->getName()
        << " | Code: " << currentUser->getCode()
        << " | Role: ";
    if (currentUser->isSuperAdminRole()) oss << "SuperAdmin";
    else if (currentUser->isAdminRole()) oss << "Admin";
    else if (currentUser->isSellerRole()) oss << "Seller";
    else if (currentUser->isWarehouseWorkerRole()) oss << "Warehouse";
    else oss << "None";
    std::string info = oss.str();
    const int width = 80;
    if ((int)info.size() < width) std::cout << std::setw(width) << info << "\n";
    else std::cout << info << "\n";
}

// Render an interactive selector and handle keyboard input (Windows-only
// single-key reads provided by readKey). Returns selected index, -1 on Esc,
// or -2 to indicate the caller should fall back to numeric input mode.
int Menu::selectFromList(const std::vector<std::string> &items, const std::string &title, int startIndex, bool allowEsc) {
    int selected = startIndex;
    if (selected < 0) selected = 0;
    if (selected >= (int)items.size()) selected = (int)items.size() - 1;

    int unknownCount = 0;
    while (true) {
        clearScreen();
        printUserBanner();
        if (!title.empty()) {
            std::cout << title << "\n\n";
        }

        for (int i = 0; i < (int)items.size(); ++i) {
            if (i == selected) {
                // Use helper that chooses ANSI or WinAPI based on console
                printHighlightedLine(std::string("  ") + items[i]);
            } else {
                std::cout << "   " << items[i] << "\n";
            }
        }

        char c = 0;
        Key k = readKey(c);
        if (k == Key::Up) {
            unknownCount = 0;
            selected = (selected - 1 + (int)items.size()) % (int)items.size();
        } else if (k == Key::Down) {
            unknownCount = 0;
            selected = (selected + 1) % (int)items.size();
        } else if (k == Key::Enter) {
            unknownCount = 0;
            return selected;
        } else if ((k == Key::Esc) && allowEsc) {
            return -1;
        } else if (k == Key::Char && c >= '1' && c <= '9') {
            int idx = (c - '1');
            if (idx >= 0 && idx < (int)items.size()) return idx;
            unknownCount = 0;
        } else {
            // Unknown key - ignore and continue. If the terminal does not
            // support single-key reads (POSIX stub) we'll receive Unknown
            // repeatedly; after a few tries, return -2 to signal caller to
            // fall back to numeric input mode.
            ++unknownCount;
            if (unknownCount > 8) return -2;
        }
    }
}

// Build and display the main menu as an interactive selector. Returns
// the mapped option number (same numbers used by processOption), -1 on
// cancel, or -2 to signal fallback to numeric input.
int Menu::interactiveSelectMainMenu() {
    std::vector<std::pair<int, std::string>> options;
    options.push_back({1, "Alta de producto"});
    options.push_back({2, "Baja de producto"});
    options.push_back({3, "Busqueda de producto"});
    options.push_back({4, "Mostrar todos los productos"});
    options.push_back({5, "Mostrar productos por tipo"});
    options.push_back({6, "Operadores"});
    options.push_back({7, "Modificar producto"});
    if (currentUser && (currentUser->isAdminRole() || currentUser->isSuperAdminRole())) {
        options.push_back({9, "Gestion de usuarios"});
    }
    if (currentUser && currentUser->isSuperAdminRole()) {
        options.push_back({10, "Asignar rol de Admin"});
    }
    if (currentUser && (currentUser->isSuperAdminRole() || currentUser->isAdminRole() || currentUser->isSellerRole())) {
        options.push_back({11, "Ventas"});
    }
    options.push_back({8, "Salir"});

    std::vector<std::string> labels;
    for (auto &p : options) labels.push_back(std::to_string(p.first) + ". " + p.second);

    int sel = selectFromList(labels, "Menu Principal", 0, true);
    if (sel == -2) return -2; // signal to caller to fall back to numeric input
    if (sel < 0) return -1;
    return options[sel].first;
}

void Menu::showMainMenu()
{
    printUserBanner();
    std::cout << std::endl;
    std::cout << "--- Menu Principal ---\n";
    // Show menu options depending on current user's roles
    if (!currentUser) {
        std::cout << "No hay usuario autenticado.\n";
        return;
    }

    bool isSuper = currentUser->isSuperAdminRole();
    bool isAdmin = currentUser->isAdminRole();
    bool isSeller = currentUser->isSellerRole();
    bool isWarehouse = currentUser->isWarehouseWorkerRole();

    if (isSuper || isAdmin || isWarehouse) std::cout << "1. Alta de producto\n";
    if (isSuper || isAdmin || isWarehouse) std::cout << "2. Baja de producto\n";
    if (isSuper || isAdmin || isSeller || isWarehouse) std::cout << "3. Busqueda de producto\n";
    if (isSuper || isAdmin || isSeller || isWarehouse) std::cout << "4. Mostrar todos los productos\n";
    if (isSuper || isAdmin || isSeller || isWarehouse) std::cout << "5. Mostrar productos por tipo\n";
    if (isSuper || isAdmin) std::cout << "6. Operadores\n";
    if (isSuper || isAdmin || isWarehouse) std::cout << "7. Modificar producto\n";
    if (isSuper || isAdmin) std::cout << "9. Gestion de usuarios\n";
    if (isSuper) std::cout << "10. Asignar rol de Admin\n";
    if (isSuper || isAdmin || isSeller) std::cout << "11. Ventas\n";
    std::cout << "8. Salir\n";
    std::cout << "Elige una opcion: ";
    std::cout << std::endl;
}

void Menu::showProductTypeMenu() const
{
    clearScreen();
    printUserBanner();
    std::cout << std::endl;
    std::cout << "-- Tipos de producto --\n";
    std::cout << "1. Electronico\n";
    std::cout << "2. Libro\n";
    std::cout << "3. Producto\n";
    std::cout << "Elige tipo: ";
}

bool Menu::processOption(int option)
{
    if (!gestor)
        return true;
    if (!currentUser) {
        std::cout << "No hay usuario autenticado.\n";
        return true;
    }

    bool isSuper = currentUser->isSuperAdminRole();
    bool isAdmin = currentUser->isAdminRole();
    bool isSeller = currentUser->isSellerRole();
    bool isWarehouse = currentUser->isWarehouseWorkerRole();

    // Permission checks per option
    if (option == 1 || option == 2) {
        if (!(isSuper || isAdmin || isWarehouse)) { std::cout << "Permisos insuficientes para esta accion.\n"; return true; }
    }
    if (option == 3) {
        if (!(isSuper || isAdmin || isSeller || isWarehouse)) { std::cout << "Permisos insuficientes para esta accion.\n"; return true; }
    }
    if (option == 4) {
        if (!(isSuper || isAdmin || isSeller || isWarehouse)) { std::cout << "Permisos insuficientes para esta accion.\n"; return true; }
    }
    if (option == 5) {
        if (!(isSuper || isAdmin || isSeller || isWarehouse)) { std::cout << "Permisos insuficientes para esta accion.\n"; return true; }
    }
    if (option == 6) {
        if (!(isSuper || isAdmin)) { std::cout << "Permisos insuficientes para esta accion.\n"; return true; }
    }
    if (option == 7) {
        if (!(isSuper || isAdmin || isWarehouse)) { std::cout << "Permisos insuficientes para esta accion.\n"; return true; }
    }
    if (option == 11) {
        if (!(isSuper || isAdmin || isSeller)) { std::cout << "Permisos insuficientes para esta accion.\n"; return true; }
    }
    // Handle logout and user management options here to keep logic centralized
    if (option == 8) {
        // logout
        currentUser = nullptr;
        std::cout << "Cerrando sesion.\n";
        waitForEnter();
        return false; // signal run() to exit menu loop
    }

    if (option == 9) {
        // Gestion de usuarios: list, add, delete
        clearScreen();
        printUserBanner();
        if (!(currentUser->isAdminRole() || currentUser->isSuperAdminRole())) {
            std::cout << "Permisos insuficientes para gestionar usuarios.\n";
            return true;
        }
        std::cout << "Gestion de usuarios " << std::endl << "1. Listar" << std::endl << "2=Agregar" << std::endl << "3=Eliminar" << std::endl;
        int choice; if (!(std::cin >> choice)) { clearInput(); return true; } clearInput();
        if (choice == 1) {
            // Print users in a table: ID | Usuario | Rol
            const int wId = 6, wName = 24, wRole = 16;
            std::cout << std::left << std::setw(wId) << "ID"
                      << std::setw(wName) << "Usuario"
                      << std::setw(wRole) << "Rol" << "\n";
            std::cout << std::string(wId + wName + wRole, '-') << "\n";
            for (const auto &usr : userManager->allUsers()) {
                std::string role = "None";
                if (usr.isSuperAdminRole()) role = "SuperAdmin";
                else if (usr.isAdminRole()) role = "Admin";
                else if (usr.isSellerRole()) role = "Seller";
                else if (usr.isWarehouseWorkerRole()) role = "Warehouse";

                std::cout << std::left << std::setw(wId) << usr.getCode()
                          << std::setw(wName) << usr.getName()
                          << std::setw(wRole) << role << "\n";
            }
        } else if (choice == 2) {
            User newu;
            int code = newu.addNewUser(*currentUser);
            if (code != 0) {
                // Add the new user to the UserManager container. We rely on
                // UserManager using a container that preserves element
                // addresses (std::list) so pointers like Menu::currentUser do
                // not become dangling after this insertion. See
                // include/UserManager.h for rationale.
                userManager->addUser(newu);
            }
        } else if (choice == 3) {
            std::cout << "Ingrese nombre de usuario a eliminar: ";
            std::string n; std::getline(std::cin, n);
            User* target = userManager->findByName(n);
            if (!target) { std::cout << "Usuario no encontrado.\n"; return true; }
            // Prevent deleting currently logged-in user
            if (target->getName() == currentUser->getName()) { std::cout << "No puede eliminar al usuario con sesion iniciada.\n"; return true; }

            // Count superadmins and admins separately to apply stricter rules
            int superAdminCount = 0;
            int adminCount = 0; // regular admin (not counting superadmins)
            // Iterate the user list and compute counts. Because the internal
            // container is a std::list the pointers/iterators used elsewhere
            // in the program remain stable during iteration and insertions.
            for (const auto &u : userManager->allUsers()) {
                if (u.isSuperAdminRole()) ++superAdminCount;
                else if (u.isAdminRole()) ++adminCount;
            }

            // If target is SuperAdmin: only a SuperAdmin can delete, and can't delete the last SuperAdmin
            if (target->isSuperAdminRole()) {
                if (!currentUser->isSuperAdminRole()) {
                    std::cout << "Operacion denegada: solo un SuperAdmin puede eliminar a otro SuperAdmin.\n";
                    return true;
                }
                if (superAdminCount <= 1) {
                    std::cout << "Operacion denegada: no se puede eliminar al ultimo SuperAdmin.\n";
                    return true;
                }
            }

            // If target is an Admin (but not SuperAdmin): only SuperAdmin can delete Admins and avoid deleting last admin
            if (target->isAdminRole() && !target->isSuperAdminRole()) {
                if (!currentUser->isSuperAdminRole()) {
                    std::cout << "Operacion denegada: solo un SuperAdmin puede eliminar a un Admin.\n";
                    return true;
                }
                if (adminCount <= 1) {
                    std::cout << "Operacion denegada: no se puede eliminar al ultimo usuario con rol Admin.\n";
                    return true;
                }
            }

            std::cout << "Confirma eliminar usuario '" << target->getName() << "'? (y/n): ";
            char c = 'n';
            if (!(std::cin >> c)) { std::cin.clear(); clearInput(); std::cout << "Operacion cancelada.\n"; return true; }
            clearInput();
            if (c == 'y' || c == 'Y') {
                bool removed = userManager->removeByName(target->getName());
                if (removed) std::cout << "Usuario eliminado.\n";
                else std::cout << "Error eliminando usuario.\n";
            } else {
                std::cout << "Operacion cancelada.\n";
            }
        }
        return true;
    }

    if (option == 10) {
        if (!currentUser->isSuperAdminRole()) { std::cout << "Permisos insuficientes.\n"; return true; }
        clearScreen();
        printUserBanner();
        std::cout << "Ingrese nombre de usuario para asignar rol Admin: ";
        std::string n; std::getline(std::cin, n);
        User* target = userManager->findByName(n);
        if (!target) { std::cout << "Usuario no encontrado.\n"; return true; }
        target->setAdmin(true);
        std::cout << "Rol Admin asignado a " << target->getName() << "\n";
        return true;
    }

    if (option == 1)
    {
        clearScreen();
        showProductTypeMenu();
        int t = 0;
        if (!(std::cin >> t))
            return true;
        clearInput();
        
        std::string name, description;
        float price = 0.0f;
        int stock = 0;

        std::cout << std::endl;
        std::cout << "Nombre: ";
        std::getline(std::cin, name);

        std::cout << "Precio: ";
        std::cin >> price;
        clearInput();
        
        std::cout << "Descripcion: ";
        std::getline(std::cin, description);
        
        std::cout << "Stock: ";
        std::cin >> stock;
        clearInput();
        
        if (t == 1)
        {
            Electronic *e = new Electronic(0, name, price, description, stock);
            std::cin >> *e;
            gestor->addProduct(e);
            std::cout << "Electronico agregado.\n";
        }
        else if (t == 2)
        {
            
            Book *b = new Book(0, name, price, description, stock);
            std::cin >> *b;
            gestor->addProduct(b);
            std::cout << "Libro agregado.\n";
        }
        else if (t == 3)
        {            
            Product *p = new Product(0, stock, price, name, description);
            gestor->addProduct(p);
            std::cout << "Producto generico agregado.\n";
        } else {
            std::cout << "Opcion no valida.\n";
        }
    }
    else if (option == 2)
    {
        clearScreen();
        printUserBanner();
        std::cout << "Ingrese ID del producto a eliminar: ";
        int id;
        if (!(std::cin >> id))
            return true;
        clearInput();
        if (gestor->removeProduct(id))
            std::cout << "Eliminado.\n";
        else
            std::cout << "No eliminado o no encontrado.\n";
    }
    else if (option == 3)
    {
        clearScreen();
        printUserBanner();
        std::cout << "Ingrese ID del producto a buscar: ";
        int id;
        if (!(std::cin >> id))
            return true;
        clearInput();
        Product *p = gestor->findProduct(id);
        if (p)
            p->showInfo();
        else
            std::cout << "Producto no encontrado.\n";
    }
    else if (option == 4)
    {
        clearScreen();
        printUserBanner();
        gestor->showAll();
    }
    else if (option == 5)
    {
        clearScreen();
        showProductTypeMenu();
        int t = 0;
        if (!(std::cin >> t))
            return true;
        clearInput();
        if (t == 1)
            gestor->showByType("Electronic");
        else if (t == 2)
            gestor->showByType("Book");
        else if(t == 3)
            gestor->showByType("Product");
        else
            std::cout << "Opcion no valida" << std::endl;

    }
    else if (option == 6)
    {
        // operatorsMenu will clear per-iteration; call it directly
        operatorsMenu();
    }
    else if (option == 11)
    {
        // salesMenu already clears each loop iteration
        salesMenu();
    }
    else if (option == 7)
    {
        clearScreen();
        modifyProduct();
    }

    std::cout << std::endl;
    return true;
}

void Menu::run()
{
    if (!userManager) {
        std::cout << "UserManager no disponible. Saliendo.\n";
        return;
    }

    // Login loop
    while (true) {
        clearScreen();
        std::string uname, pwd;
        
        std::cout << "Usuario (o 'exit' para salir): ";
        std::getline(std::cin, uname);
        if (uname == "exit") return;
        
        std::cout << "Password: ";
        pwd = readPassword();

        User* u = userManager->authenticate(uname, pwd);
        if (!u) {
            std::cout << "Credenciales invalidas. Intente otra vez.\n";
            waitForEnter();
            continue;
        }
        currentUser = u;
        
        std::cout << "Bienvenido, " << currentUser->getName() << "\n";
        waitForEnter();
        break;
    }

    bool exit = false;
    while (!exit)
    {
        // Try interactive selector first (arrow keys + Enter). If the
        // terminal doesn't support single-key reads, interactiveSelectMainMenu
        // will return -2 and we fall back to numeric input.
        int selected = interactiveSelectMainMenu();
        int opt = 0;
        if (selected == -2) {
            // fallback to numeric
            clearScreen();
            showMainMenu();
            if (!(std::cin >> opt)) break;
            clearInput();
        } else if (selected == -1) {
            // treat ESC as exit (map to option 8 / logout)
            opt = 8;
        } else {
            opt = selected;
        }

        if (!processOption(opt))
            break;
        // after action, allow user to see the output before clearing
        std::cout << std::endl;
        waitForEnter();
    }
}

static std::string readLineAllowEmpty(const std::string &prompt)
{
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

void Menu::modifyProduct()
{
    if (!gestor) return;
    clearScreen();
    printUserBanner();
    std::cout << "Ingrese ID del producto a modificar: ";
    int id;
    if (!(std::cin >> id)) { clearInput(); return; }
    clearInput();
    Product* p = gestor->findProduct(id);
    if (!p) { std::cout << "Producto no encontrado.\n"; return; }

    p->showInfo();

    std::string s;
    s = readLineAllowEmpty("Nuevo nombre (enter = mantener): ");
    if (!s.empty()) p->setName(s);

    s = readLineAllowEmpty("Nuevo precio (enter = mantener): ");
    if (!s.empty()) {
        try { float v = std::stof(s); p->setPrice(v); } catch(...) {}
    }

    s = readLineAllowEmpty("Nueva descripcion (enter = mantener): ");
    if (!s.empty()) p->setDescription(s);

    s = readLineAllowEmpty("Nuevo stock (enter = mantener): ");
    if (!s.empty()) { try { int v = std::stoi(s); p->setStock(v); } catch(...) {} }

    Electronic* e = dynamic_cast<Electronic*>(p);
    if (e)
    {
        s = readLineAllowEmpty("Nueva marca (enter = mantener): "); if (!s.empty()) e->setBrand(s);
        s = readLineAllowEmpty("Nuevo modelo (enter = mantener): "); if (!s.empty()) e->setModel(s);
        s = readLineAllowEmpty("Nueva garantia meses (enter = mantener): "); if (!s.empty()){ try{ int v = std::stoi(s); e->setWarrantyMonths(v);}catch(...){} }
        s = readLineAllowEmpty("Nuevas especificaciones (enter = mantener): "); if (!s.empty()) e->setSpecifications(s);
    }

    Book* b = dynamic_cast<Book*>(p);
    if (b)
    {
        s = readLineAllowEmpty("Nuevo autor (enter = mantener): "); if (!s.empty()) b->setAuthor(s);
        s = readLineAllowEmpty("Nueva editorial (enter = mantener): "); if (!s.empty()) b->setPublisher(s);
        s = readLineAllowEmpty("Nuevo ISBN (enter = mantener): "); if (!s.empty()) b->setIsbn(s);
        s = readLineAllowEmpty("Nuevas paginas (enter = mantener): "); if (!s.empty()){ try{ int v = std::stoi(s); b->setPages(v);}catch(...){} }
        s = readLineAllowEmpty("Nuevo genero (enter = mantener): "); if (!s.empty()) b->setGenre(s);
    }

    std::cout << "Producto modificado.\n";
}

void Menu::showOperatorsMenu() const
{
    std::cout << std::endl;
    std::cout << "-- Menu Operadores --\n";
    std::cout << "1. Comparar si los ID son iguales (==)\n";
    std::cout << "2. Comparar si los ID no son iguales (!=)\n";
    std::cout << "3. Determinar cual el menor precio (<)\n";
    std::cout << "4. Determinar cual tiene el mayor precio (>)\n";
    std::cout << "5. Sumar productos (+)\n";
    std::cout << "6. Volver\n";
    std::cout << "Elige una opcion: ";
}

void Menu::operatorsMenu()
{
    if (!gestor) return;
    while (true)
    {
        clearScreen();
        printUserBanner();
        showOperatorsMenu();
        int op = 0;
        if (!(std::cin >> op)) return;
        clearInput();
        if (op == 6) return;

        int id1 = 0, id2 = 0;
        std::cout << "Ingrese ID del primer producto: ";
        if (!(std::cin >> id1)) { clearInput(); continue; }
        clearInput();
        std::cout << "Ingrese ID del segundo producto: ";
        if (!(std::cin >> id2)) { clearInput(); continue; }
        clearInput();

        Product* p1 = gestor->findProduct(id1);
        Product* p2 = gestor->findProduct(id2);
        if (!p1 || !p2)
        {
            std::cout << "Uno o ambos productos no encontrados.\n";
            continue;
        }

        if (op == 1)
        {
            bool res = (*p1) == (*p2);
            std::cout << (res ? "Productos iguales\n" : "Productos distintos\n");
        }
        else if (op == 2)
        {
            bool res = (*p1) != (*p2);
            std::cout << (res ? "Productos distintos\n" : "Productos iguales\n");
        }
        else if (op == 3)
        {
            bool res = (*p1) < (*p2);
            std::cout << (res ? "El precio del primer producto es menor\n" : "El precio del segundo producto es menor\n");
        }
        else if (op == 4)
        {
            bool res = (*p1) > (*p2);
            std::cout << (res ? "El precio del primer producto es mayor\n" : "El precio del segundo producto es mayor\n");
        }
        else if (op == 5)
        {
            Product combined = (*p1) + (*p2);
            std::cout << "Resultado de la suma:\n";
            std::cout << combined << std::endl;
        }
    }
}

void Menu::salesMenu()
{
    while (true) {
        clearScreen();
        printUserBanner();
        std::cout << "-- Modulo Ventas --\n";
        std::cout << "1. Nueva venta\n";
        std::cout << "2. Buscar venta\n";
        std::cout << "3. Volver\n";
        std::cout << "Elige una opcion: ";
        int op = 0;
        if (!(std::cin >> op)) return;
        clearInput();
        if (op == 3) return;
        if (op == 1) {
            // Create sale
            std::cout << "Ingrese ID del producto a vender: ";
            int pid; if (!(std::cin >> pid)) { clearInput(); std::cout << "ID invalido.\n"; continue; } clearInput();
            Product* prod = gestor->findProduct(pid);
            if (!prod) { std::cout << "Producto no encontrado.\n"; continue; }
            std::cout << "Producto: " << prod->getName() << "\n";
            std::cout << "Ingrese unidades vendidas: ";
            int units; if (!(std::cin >> units)) { clearInput(); std::cout << "Unidades invalidas.\n"; continue; } clearInput();
            if (units <= 0) { std::cout << "La cantidad debe ser mayor que 0.\n"; waitForEnter(); continue; }
            if (units > prod->getStock()) { std::cout << "Stock insuficiente. Stock actual: " << prod->getStock() << "\n"; waitForEnter(); continue; }
            float total = units * prod->getPrice();
            std::string ptype = "Producto";
            if (dynamic_cast<Electronic*>(prod)) ptype = "Electronic";
            else if (dynamic_cast<Book*>(prod)) ptype = "Book";
            Sale s(0, currentUser->getCode(), currentUser->getName(), ptype, prod->getName(), units, total);

            // show summary and ask for confirmation
            std::cout << "Resumen de venta:\n";
            std::cout << "Usuario: " << s.userName << " (code=" << s.userCode << ")\n";
            std::cout << "Producto: [" << s.productType << "] " << s.productName << " x" << s.units << " => Total: " << std::fixed << std::setprecision(2) << s.totalPrice << "\n";
            std::cout << "Confirmar venta? (y/n): ";
            char confirm = 'n';
            if (!(std::cin >> confirm)) { std::cin.clear(); clearInput(); std::cout << "Entrada invalida. Venta cancelada.\n"; waitForEnter(); continue; }
            clearInput();
            if (!(confirm == 'y' || confirm == 'Y')) { std::cout << "Venta cancelada.\n"; waitForEnter(); continue; }

            // proceed to register sale and decrement stock
            int saleId = 0;
            // decrement stock
            prod->setStock(prod->getStock() - units);
            if (salesManager) saleId = salesManager->addSale(s);

            // print as table
            const int wId = 6, wUcode = 10, wUname = 16, wPtype = 12, wPname = 22, wUnits = 8, wTotal = 12;
            std::ostringstream oss; oss << std::fixed << std::setprecision(2) << s.totalPrice;
            std::cout << std::left << std::setw(wId) << "ID"
                      << std::setw(wUcode) << "UCode"
                      << std::setw(wUname) << "Usuario"
                      << std::setw(wPtype) << "Tipo"
                      << std::setw(wPname) << "Producto"
                      << std::setw(wUnits) << "Unids"
                      << std::setw(wTotal) << "Total" << "\n";
            std::cout << std::string(wId+wUcode+wUname+wPtype+wPname+wUnits+wTotal, '-') << "\n";
            std::cout << std::left << std::setw(wId) << saleId
                      << std::setw(wUcode) << s.userCode
                      << std::setw(wUname) << s.userName
                      << std::setw(wPtype) << s.productType
                      << std::setw(wPname) << s.productName
                      << std::setw(wUnits) << s.units
                      << std::setw(wTotal) << oss.str() << "\n";
            std::cout << "Venta registrada. ID venta: " << saleId << "\n";
            // details already printed in table
        } else if (op == 2) {
            // Behavior depends on user role
            bool isSeller = currentUser->isSellerRole();
            bool isAdmin = currentUser->isAdminRole();
            bool isSuper = currentUser->isSuperAdminRole();

            if (isSeller && !isAdmin && !isSuper) {
                // Seller: directly show their own sales
                int ucode = currentUser->getCode();
                auto list = salesManager ? salesManager->findByUserCode(ucode) : std::vector<Sale>();
                if (list.empty()) {
                    std::cout << "No se encontraron ventas para su usuario.\n";
                } else {
                    const int wId = 6, wUcode = 10, wUname = 16, wPtype = 12, wPname = 22, wUnits = 8, wTotal = 12;
                    std::cout << std::left << std::setw(wId) << "ID"
                              << std::setw(wUcode) << "UCode"
                              << std::setw(wUname) << "Usuario"
                              << std::setw(wPtype) << "Tipo"
                              << std::setw(wPname) << "Producto"
                              << std::setw(wUnits) << "Unids"
                              << std::setw(wTotal) << "Total" << "\n";
                    std::cout << std::string(wId+wUcode+wUname+wPtype+wPname+wUnits+wTotal, '-') << "\n";
                    for (const auto &fs : list) {
                        std::ostringstream oss; oss << std::fixed << std::setprecision(2) << fs.totalPrice;
                        std::cout << std::left << std::setw(wId) << fs.saleId
                                  << std::setw(wUcode) << fs.userCode
                                  << std::setw(wUname) << fs.userName
                                  << std::setw(wPtype) << fs.productType
                                  << std::setw(wPname) << fs.productName
                                  << std::setw(wUnits) << fs.units
                                  << std::setw(wTotal) << oss.str() << "\n";
                    }
                }
            } else if (isAdmin || isSuper) {
                // Admin: give options to view all sales or filter by user or by ID
                std::cout << "Buscar ventas: 1=Todas las ventas, 2=Filtrar por codigo de usuario, 3=Buscar por ID de venta: ";
                int choice; if (!(std::cin >> choice)) { clearInput(); std::cout << "Opcion invalida.\n"; continue; } clearInput();
                if (choice == 1) {
                    auto list = salesManager ? salesManager->allSales() : std::vector<Sale>();
                    if (list.empty()) { std::cout << "No hay ventas registradas.\n"; }
                    else {
                        const int wId = 6, wUcode = 10, wUname = 16, wPtype = 12, wPname = 22, wUnits = 8, wTotal = 12;
                        std::cout << std::left << std::setw(wId) << "ID"
                                  << std::setw(wUcode) << "UCode"
                                  << std::setw(wUname) << "Usuario"
                                  << std::setw(wPtype) << "Tipo"
                                  << std::setw(wPname) << "Producto"
                                  << std::setw(wUnits) << "Unids"
                                  << std::setw(wTotal) << "Total" << "\n";
                        std::cout << std::string(wId+wUcode+wUname+wPtype+wPname+wUnits+wTotal, '-') << "\n";
                        for (const auto &fs : list) {
                            std::ostringstream oss; oss << std::fixed << std::setprecision(2) << fs.totalPrice;
                            std::cout << std::left << std::setw(wId) << fs.saleId
                                      << std::setw(wUcode) << fs.userCode
                                      << std::setw(wUname) << fs.userName
                                      << std::setw(wPtype) << fs.productType
                                      << std::setw(wPname) << fs.productName
                                      << std::setw(wUnits) << fs.units
                                      << std::setw(wTotal) << oss.str() << "\n";
                        }
                    }
                } else if (choice == 2) {
                    std::cout << "Ingrese codigo de usuario: "; int ucode; if (!(std::cin >> ucode)) { clearInput(); std::cout << "Codigo invalido.\n"; continue; } clearInput();
                    auto list = salesManager ? salesManager->findByUserCode(ucode) : std::vector<Sale>();
                    if (list.empty()) { std::cout << "No se encontraron ventas para ese usuario.\n"; }
                    else {
                        const int wId = 6, wUcode = 10, wUname = 16, wPtype = 12, wPname = 22, wUnits = 8, wTotal = 12;
                        std::cout << std::left << std::setw(wId) << "ID"
                                  << std::setw(wUcode) << "UCode"
                                  << std::setw(wUname) << "Usuario"
                                  << std::setw(wPtype) << "Tipo"
                                  << std::setw(wPname) << "Producto"
                                  << std::setw(wUnits) << "Unids"
                                  << std::setw(wTotal) << "Total" << "\n";
                        std::cout << std::string(wId+wUcode+wUname+wPtype+wPname+wUnits+wTotal, '-') << "\n";
                        for (const auto &fs : list) {
                            std::ostringstream oss; oss << std::fixed << std::setprecision(2) << fs.totalPrice;
                            std::cout << std::left << std::setw(wId) << fs.saleId
                                      << std::setw(wUcode) << fs.userCode
                                      << std::setw(wUname) << fs.userName
                                      << std::setw(wPtype) << fs.productType
                                      << std::setw(wPname) << fs.productName
                                      << std::setw(wUnits) << fs.units
                                      << std::setw(wTotal) << oss.str() << "\n";
                        }
                    }
                } else if (choice == 3) {
                    std::cout << "Ingrese ID de venta: "; int sid; if (!(std::cin >> sid)) { clearInput(); std::cout << "ID invalido.\n"; continue; } clearInput();
                    Sale* found = salesManager ? salesManager->findById(sid) : nullptr;
                    if (!found) { std::cout << "Venta no encontrada.\n"; }
                    else {
                        const int wId = 6, wUcode = 10, wUname = 16, wPtype = 12, wPname = 22, wUnits = 8, wTotal = 12;
                        std::ostringstream oss; oss << std::fixed << std::setprecision(2) << found->totalPrice;
                        std::cout << std::left << std::setw(wId) << "ID"
                                  << std::setw(wUcode) << "UCode"
                                  << std::setw(wUname) << "Usuario"
                                  << std::setw(wPtype) << "Tipo"
                                  << std::setw(wPname) << "Producto"
                                  << std::setw(wUnits) << "Unids"
                                  << std::setw(wTotal) << "Total" << "\n";
                        std::cout << std::string(wId+wUcode+wUname+wPtype+wPname+wUnits+wTotal, '-') << "\n";
                        std::cout << std::left << std::setw(wId) << found->saleId
                                  << std::setw(wUcode) << found->userCode
                                  << std::setw(wUname) << found->userName
                                  << std::setw(wPtype) << found->productType
                                  << std::setw(wPname) << found->productName
                                  << std::setw(wUnits) << found->units
                                  << std::setw(wTotal) << oss.str() << "\n";
                    }
                } else {
                    std::cout << "Opcion no valida.\n";
                }
            } else {
                // For other roles, fallback to previous behavior (allow searching by ID or user code)
                std::cout << "Buscar por: 1=ID venta, 2=Codigo usuario: ";
                int choice; if (!(std::cin >> choice)) { clearInput(); std::cout << "Opcion invalida.\n"; continue; } clearInput();
                if (choice == 1) {
                    std::cout << "Ingrese ID de venta: "; int sid; if (!(std::cin >> sid)) { clearInput(); std::cout << "ID invalido.\n"; continue; } clearInput();
                    Sale* found = salesManager ? salesManager->findById(sid) : nullptr;
                    if (!found) { std::cout << "Venta no encontrada.\n"; }
                    else {
                        const int wId = 6, wUcode = 10, wUname = 16, wPtype = 12, wPname = 22, wUnits = 8, wTotal = 12;
                        std::ostringstream oss; oss << std::fixed << std::setprecision(2) << found->totalPrice;
                        std::cout << std::left << std::setw(wId) << "ID"
                                  << std::setw(wUcode) << "UCode"
                                  << std::setw(wUname) << "Usuario"
                                  << std::setw(wPtype) << "Tipo"
                                  << std::setw(wPname) << "Producto"
                                  << std::setw(wUnits) << "Unids"
                                  << std::setw(wTotal) << "Total" << "\n";
                        std::cout << std::string(wId+wUcode+wUname+wPtype+wPname+wUnits+wTotal, '-') << "\n";
                        std::cout << std::left << std::setw(wId) << found->saleId
                                  << std::setw(wUcode) << found->userCode
                                  << std::setw(wUname) << found->userName
                                  << std::setw(wPtype) << found->productType
                                  << std::setw(wPname) << found->productName
                                  << std::setw(wUnits) << found->units
                                  << std::setw(wTotal) << oss.str() << "\n";
                    }
                } else if (choice == 2) {
                    std::cout << "Ingrese codigo de usuario: "; int ucode; if (!(std::cin >> ucode)) { clearInput(); std::cout << "Codigo invalido.\n"; continue; } clearInput();
                    auto list = salesManager ? salesManager->findByUserCode(ucode) : std::vector<Sale>();
                    if (list.empty()) { std::cout << "No se encontraron ventas para ese usuario.\n"; }
                    else {
                        const int wId = 6, wUcode = 10, wUname = 16, wPtype = 12, wPname = 22, wUnits = 8, wTotal = 12;
                        std::cout << std::left << std::setw(wId) << "ID"
                                  << std::setw(wUcode) << "UCode"
                                  << std::setw(wUname) << "Usuario"
                                  << std::setw(wPtype) << "Tipo"
                                  << std::setw(wPname) << "Producto"
                                  << std::setw(wUnits) << "Unids"
                                  << std::setw(wTotal) << "Total" << "\n";
                        std::cout << std::string(wId+wUcode+wUname+wPtype+wPname+wUnits+wTotal, '-') << "\n";
                        for (const auto &fs : list) {
                            std::ostringstream oss; oss << std::fixed << std::setprecision(2) << fs.totalPrice;
                            std::cout << std::left << std::setw(wId) << fs.saleId
                                      << std::setw(wUcode) << fs.userCode
                                      << std::setw(wUname) << fs.userName
                                      << std::setw(wPtype) << fs.productType
                                      << std::setw(wPname) << fs.productName
                                      << std::setw(wUnits) << fs.units
                                      << std::setw(wTotal) << oss.str() << "\n";
                        }
                    }
                } else {
                    std::cout << "Opcion no valida.\n";
                }
            }
        } else {
            std::cout << "Opcion no valida.\n";
        }
        waitForEnter();
    }
}
