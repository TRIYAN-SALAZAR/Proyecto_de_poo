#include <iostream>
#include "Gestor.h"
#include "Menu.h"
#include "Electronic.h"
#include "Book.h"
#include "UserManager.h"
#include "User.h"
#include "SalesManager.h"

int main()
{
    Gestor gestor;

    Electronic *e1 = new Electronic(0, "Smartphone", 299.99f, "Multifunctional phone", 15, "Acme", "X100", 24, "Specs...");
    Electronic *e2 = new Electronic(0, "Laptop", 899.50f, "Lightweight laptop", 8, "Acme", "BookPro", 12, "Specs...");
    Electronic *e3 = new Electronic(0, "Wireless Headphones", 59.99f, "Noise-cancelling", 25, "SoundCo", "WH-200", 6, "Specs...");
    Electronic *e4 = new Electronic(0, "Smartwatch", 129.00f, "Fitness tracker + notifications", 30, "WristTech", "FitX", 12, "Specs...");
    Electronic *e5 = new Electronic(0, "Tablet", 249.99f, "10-inch tablet", 12, "TabWorks", "Tab10", 12, "Specs...");
    Electronic *e6 = new Electronic(0, "External HDD 1TB", 74.50f, "Portable storage", 18, "StoragePro", "HDD1T", 24, "Specs...");

    Book *b1 = new Book(0, "C++ Programming", 39.90f, "Learn C++", 20, "John Doe", "TechPub", "978-0-123456-47-2", 450, "Programming");
    Book *b2 = new Book(0, "Data Structures", 29.50f, "Core data structures", 10, "Jane Smith", "UniPress", "978-1-234567-89-7", 320, "Computer Science");
    Book *b3 = new Book(0, "Algorithms", 34.75f, "Algorithm design", 8, "A. Author", "AlgoPub", "978-0-987654-32-1", 500, "Computer Science");
    Book *b4 = new Book(0, "Design Patterns", 45.00f, "Reusable OO designs", 6, "Gamma", "PatternHouse", "978-0-111111-11-1", 395, "Software Engineering");

    Product *p1 = new Product(0, 50, 4.99f, "USB Cable", "1m USB-A to USB-C cable");
    Product *p2 = new Product(0, 40, 9.99f, "Mouse", "Optical USB mouse");

    gestor.addProduct(e1);
    gestor.addProduct(e2);
    gestor.addProduct(e3);
    gestor.addProduct(e4);
    gestor.addProduct(e5);
    gestor.addProduct(e6);
    gestor.addProduct(b1);
    gestor.addProduct(b2);
    gestor.addProduct(b3);
    gestor.addProduct(b4);
    gestor.addProduct(p1);
    gestor.addProduct(p2);

    // Setup users
    UserManager userManager;
    User superu;
    superu.setName("superadmin");
    superu.setPassword("root");
    superu.setSuperAdmin(true);
    superu.setAdmin(true);
    userManager.addUser(superu);

    User admin;
    admin.setName("admin");
    admin.setPassword("admin");
    admin.setAdmin(true);
    userManager.addUser(admin);

    User seller;
    seller.setName("seller");
    seller.setPassword("seller");
    seller.setSeller(true);
    userManager.addUser(seller);

    User worker;
    worker.setName("worker");
    worker.setPassword("worker");
    worker.setWarehouseWorker(true);
    userManager.addUser(worker);

    SalesManager salesManager;

    Menu menu(&gestor, &userManager, &salesManager);
    menu.run();

    return 0;
}
