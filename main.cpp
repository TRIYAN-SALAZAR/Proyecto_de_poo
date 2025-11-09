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

    Electronic* e1 = new Electronic(0, "Smartphone", 299.99f, "Multifunctional phone", 15, "Acme", "X100", 24, "Specs...");
    Electronic* e2 = new Electronic(0, "Laptop", 899.50f, "Lightweight laptop", 8, "Acme", "BookPro", 12, "Specs...");

    Book* b1 = new Book(0, "C++ Programming", 39.90f, "Learn C++", 20, "John Doe", "TechPub", "978-0-123456-47-2", 450, "Programming");

    gestor.addProduct(e1);
    gestor.addProduct(e2);
    gestor.addProduct(b1);

    // Setup users
    UserManager userManager;
    User superu; superu.setName("superadmin"); superu.setPassword("root"); superu.setSuperAdmin(true); superu.setAdmin(true);
    userManager.addUser(superu);

    User admin; admin.setName("admin"); admin.setPassword("admin"); admin.setAdmin(true);
    userManager.addUser(admin);

    User seller; seller.setName("seller"); seller.setPassword("seller"); seller.setSeller(true);
    userManager.addUser(seller);

    User worker; worker.setName("worker"); worker.setPassword("worker"); worker.setWarehouseWorker(true);
    userManager.addUser(worker);

    SalesManager salesManager;

    Menu menu(&gestor, &userManager, &salesManager);
    menu.run();

    return 0;
}
