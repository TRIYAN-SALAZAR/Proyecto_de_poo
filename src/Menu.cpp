#include "Menu.h"
#include <iostream>
#include <limits>
#include "Electronic.h"
#include "Book.h"

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
    std::cout << "6. Salir\n";
    std::cout << "Elige una opcion: ";
    std::cout << std::endl;
}

void Menu::showProductTypeMenu() const
{
    std::cout << std::endl;
    std::cout << "-- Tipos de producto --\n";
    std::cout << "1. Electronico\n";
    std::cout << "2. Libro\n";
    std::cout << "3. Producto generico\n";
    std::cout << "Elige tipo: ";
}

static void clearInput()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
        if (opt == 6)
            break;
        processOption(opt);
    }
}
