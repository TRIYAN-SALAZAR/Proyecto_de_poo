#include <iostream>
#include <limits>
#include "Menu.h"
#include "Electronic.h"
#include "Book.h"
#include "UTILITIES.h"

Menu::Menu(Gestor *gestor)
    : gestor(gestor)
{
}

void Menu::showMainMenu() const
{
    std::cout << std::endl;
    std::cout << "--- Menu Principal ---\n";
    std::cout << "1. Alta de producto\n";
    std::cout << "2. Baja de producto\n";
    std::cout << "3. Busqueda de producto\n";
    std::cout << "4. Mostrar todos los productos\n";
    std::cout << "5. Mostrar productos por tipo\n";
    std::cout << "6. Operadores\n";
    std::cout << "7. Modificar producto\n";
    std::cout << "8. Salir\n";
    std::cout << "Elige una opcion: ";
    std::cout << std::endl;
}

void Menu::showProductTypeMenu() const
{
    std::cout << std::endl;
    std::cout << "-- Tipos de producto --\n";
    std::cout << "1. Electronico\n";
    std::cout << "2. Libro\n";
    std::cout << "Elige tipo: ";
}

void Menu::processOption(int option)
{
    if (!gestor)
        return;
    if (option == 1)
    {
        showProductTypeMenu();
        int t = 0;
        if (!(std::cin >> t))
            return;
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
        std::cout << std::endl;
        
        if (t == 1)
        {
            std::string brand, model, specs;
            int warranty = 0;
            std::cout << "Marca: ";
            std::getline(std::cin, brand);
            std::cout << "Modelo: ";
            std::getline(std::cin, model);
            std::cout << "Garantia meses: ";
            std::cin >> warranty;
            clearInput();
            std::cout << "Especificaciones: ";
            std::getline(std::cin, specs);
            Electronic *e = new Electronic(0, name, price, description, stock, brand, model, warranty, specs);
            gestor->addProduct(e);
            std::cout << "Electronico anadido.\n";
        }
        else if (t == 2)
        {
            std::string author, publisher, isbn, genre;
            int pages = 0;
            std::cout << "Autor: ";
            std::getline(std::cin, author);
            std::cout << "Editorial: ";
            std::getline(std::cin, publisher);
            std::cout << "ISBN: ";
            std::getline(std::cin, isbn);
            std::cout << "Paginas: ";
            std::cin >> pages;
            clearInput();
            std::cout << "Genero: ";
            std::getline(std::cin, genre);
            Book *b = new Book(0, name, price, description, stock, author, publisher, isbn, pages, genre);
            gestor->addProduct(b);
            std::cout << "Libro anadido.\n";
        }
        else
        {
            Product *p = new Product(0, stock, price, name, description);
            gestor->addProduct(p);
            std::cout << "Producto generico anadido.\n";
        }
    }
    else if (option == 2)
    {
        std::cout << "Ingrese ID del producto a eliminar: ";
        int id;
        if (!(std::cin >> id))
            return;
        clearInput();
        if (gestor->removeProduct(id))
            std::cout << "Eliminado.\n";
        else
            std::cout << "No eliminado o no encontrado.\n";
    }
    else if (option == 3)
    {
        std::cout << "Ingrese ID del producto a buscar: ";
        int id;
        if (!(std::cin >> id))
            return;
        clearInput();
        Product *p = gestor->findProduct(id);
        if (p)
            p->showInfo();
        else
            std::cout << "Producto no encontrado.\n";
    }
    else if (option == 4)
    {
        gestor->showAll();
    }
    else if (option == 5)
    {
        showProductTypeMenu();
        int t = 0;
        if (!(std::cin >> t))
            return;
        clearInput();
        if (t == 1)
            gestor->showByType("Electronic");
        else if (t == 2)
            gestor->showByType("Book");
    }
    else if (option == 6)
    {
        operatorsMenu();
    }
    else if (option == 7)
    {
        modifyProduct();
    }

    std::cout << std::endl;
}

void Menu::run()
{
    bool exit = false;
    while (!exit)
    {
        showMainMenu();
        int opt = 0;
        if (!(std::cin >> opt))
            break;
        if (opt == 8)
            break;
        processOption(opt);
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
