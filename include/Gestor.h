#ifndef GESTOR_H
#define GESTOR_H

#include <vector>
#include <string>
#include <Product.h>

class Gestor
{
    private:
        std::vector<Product *> list;
        int counterId;

    public:
        Gestor();
        ~Gestor();

        void addProduct(Product *nuevo);
        Product *findProduct(int id) const;
        bool removeProduct(int id);
        void showAll() const;
        void showByType(const std::string &tipo) const;
        int generateId();
};

#endif // GESTOR_H
