#include "Gestor.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include "Electronic.h"
#include "Book.h"

Gestor::Gestor()
    : list(), counterId(0)
{
}

Gestor::~Gestor()
{
    for (Product *p : list)
    {
        delete p;
    }
    list.clear();
    counterId = 0;
}

void Gestor::addProduct(Product *nuevo)
{
    if (!nuevo)
        return;
    if (nuevo->getId() <= 0)
    {
        nuevo->setId(generateId());
    }
    list.push_back(nuevo);
}

Product *Gestor::findProduct(int id) const
{
    for (Product *p : list)
    {
        if (p && p->getId() == id)
            return p;
    }
    return nullptr;
}

bool Gestor::removeProduct(int id)
{
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        Product *p = *it;
        if (p && p->getId() == id)
        {
            std::cout << "Seguro que quiere eliminar el producto?" << id << " (y/n): ";
            char c = 'n';
            if (!(std::cin >> c))
                return false;
            if (c == 'y' || c == 'Y')
            {
                delete p;
                list.erase(it);
                return true;
            }
            return false;
        }
    }
    return false;
}

void Gestor::showAll() const
{
    const int wId = 6;
    const int wTipo = 14;
    const int wNombre = 22;
    const int wPrecio = 10;
    const int wStock = 8;
    const int wDesc = 36;

    auto truncate = [](const std::string &s, std::size_t w) -> std::string
    {
        if (s.size() <= w)
            return s;
        return s.substr(0, w - 3) + "...";
    };

    std::cout << std::endl;
    std::cout << std::left << std::setw(wId) << "ID"
              << std::setw(wTipo) << "Tipo"
              << std::setw(wNombre) << "Nombre"
              << std::setw(wPrecio) << "Precio"
              << std::setw(wStock) << "Stock"
              << std::setw(wDesc) << "Descripcion" << "\n";

    std::cout << std::string(wId + wTipo + wNombre + wPrecio + wStock + wDesc, '-') << "\n";

    for (const Product *p : list)
    {
        if (!p) continue;
        std::cout << *p << "\n";
    }

    std::cout << std::endl;
}

void Gestor::showByType(const std::string &tipo) const
{
    auto truncate = [](const std::string &s, std::size_t w) -> std::string {
        if (s.size() <= w)
            return s;
        return s.substr(0, w - 3) + "...";
    };

    bool wantElectronic = (tipo == "Electronic" || tipo == "Electronico" || tipo == "1");
    bool wantBook = (tipo == "Book" || tipo == "Libro" || tipo == "2");
    bool wantGeneric = (tipo == "Product" || tipo == "Producto" || tipo == "3");
    bool wantAll = (!wantElectronic && !wantBook && !wantGeneric) || tipo == "All" || tipo == "Producto" || tipo == "Product";

    if (wantElectronic)
    {
        const int wId = 6; const int wMarca = 14; const int wModelo = 12; const int wNombre = 20; const int wPrecio = 10; const int wStock = 8; const int wGarantia = 10; const int wSpecs = 28;
        std::cout << std::endl;
        std::cout << std::left << std::setw(wId) << "ID"
                  << std::setw(wMarca) << "Marca"
                  << std::setw(wModelo) << "Modelo"
                  << std::setw(wNombre) << "Nombre"
                  << std::setw(wPrecio) << "Precio"
                  << std::setw(wStock) << "Stock"
                  << std::setw(wGarantia) << "Garantia"
                  << std::setw(wSpecs) << "Especificaciones" << "\n";
        std::cout << std::string(wId + wMarca + wModelo + wNombre + wPrecio + wStock + wGarantia + wSpecs, '-') << "\n";

        for (const Product *p : list)
        {
            if (!p) continue;
            const Electronic* e = dynamic_cast<const Electronic*>(p);
            if (!e) continue;
            std::cout << *e << "\n";
        }
        std::cout << std::endl;
        return;
    }

    if (wantBook)
    {
        const int wId = 6; const int wAutor = 18; const int wTitulo = 20; const int wEditorial = 16; const int wisbn = 16; const int wPag = 8; const int wPrecio = 10; const int wStock = 8;
        std::cout << std::endl;
        std::cout << std::left << std::setw(wId) << "ID"
                  << std::setw(wAutor) << "Autor"
                  << std::setw(wTitulo) << "Titulo"
                  << std::setw(wEditorial) << "Editorial"
                  << std::setw(wisbn) << "ISBN"
                  << std::setw(wPag) << "Paginas"
                  << std::setw(wPrecio) << "Precio"
                  << std::setw(wStock) << "Stock" << "\n";
        std::cout << std::string(wId + wAutor + wTitulo + wEditorial + wisbn + wPag + wPrecio + wStock, '-') << "\n";

        for (const Product *p : list)
        {
            if (!p) continue;
            const Book* b = dynamic_cast<const Book*>(p);
            if (!b) continue;
            std::cout << *b << "\n";
        }
        std::cout << std::endl;
        return;
    }

    // Generic/Product or all
    const int wId = 6;
    const int wTipo = 14;
    const int wNombre = 22;
    const int wPrecio = 10;
    const int wStock = 8;
    const int wDesc = 36;

    std::cout << std::endl;
    std::cout << std::left << std::setw(wId) << "ID"
              << std::setw(wTipo) << "Tipo"
              << std::setw(wNombre) << "Nombre"
              << std::setw(wPrecio) << "Precio"
              << std::setw(wStock) << "Stock"
              << std::setw(wDesc) << "Descripcion" << "\n";

    std::cout << std::string(wId + wTipo + wNombre + wPrecio + wStock + wDesc, '-') << "\n";

    for (const Product *p : list)
    {
        if (!p) continue;
        std::string tipoStr = "Producto";
        if (dynamic_cast<const Electronic *>(p)) tipoStr = "Electronico";
        else if (dynamic_cast<const Book *>(p)) tipoStr = "Libro";
        std::ostringstream oss; oss << std::fixed << std::setprecision(2) << p->getPrice();
        std::cout << std::left << std::setw(wId) << p->getId()
                  << std::setw(wTipo) << tipoStr
                  << std::setw(wNombre) << truncate(p->getName(), wNombre - 1)
                  << std::setw(wPrecio) << oss.str()
                  << std::setw(wStock) << p->getStock()
                  << std::setw(wDesc) << truncate(p->getDescription(), wDesc - 1)
                  << "\n";
    }

    std::cout << std::endl;
}

int Gestor::generateId()
{
    return ++counterId;
}
