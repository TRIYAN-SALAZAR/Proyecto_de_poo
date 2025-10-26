#include "Product.h"
#include "Book.h"
#include "Electronic.h"
#include <iostream>
#include <limits>
#include <iomanip>

Product::Product()
    : id(0), stock(0), price(0.0f), name(""), description("")
{
}

Product::Product(int id, int stock, float price, const std::string &name, const std::string &description)
    : id(id), stock(stock), price(price), name(name), description(description)
{
}

int Product::getId() const { return id; }
void Product::setId(int id) { this->id = id; }

std::string Product::getName() const { return name; }
void Product::setName(const std::string &name) { this->name = name; }

float Product::getPrice() const { return price; }
void Product::setPrice(float price) { this->price = price; }

std::string Product::getDescription() const { return description; }
void Product::setDescription(const std::string &description) { this->description = description; }

int Product::getStock() const { return stock; }
void Product::setStock(int stock) { this->stock = stock; }

void Product::showInfo() const
{
    std::cout << "Producto ID: " << id << "\n"
              << "Nombre: " << name << "\n"
              << "Descripcion: " << description << "\n"
              << "Precio: " << price << "\n"
              << "Stock: " << stock << std::endl;
}

float Product::calculateFinalPrice() const
{
    return price;
}

Product::~Product() = default;

bool Product::operator==(const Product &p) const
{
    return this->id == p.id;
}

bool Product::operator!=(const Product &p) const
{
    return !(*this == p);
}

bool Product::operator<(const Product &p) const
{
    if (this->price != p.price)
        return this->price < p.price;
    return this->id < p.id;
}

bool Product::operator>(const Product &p) const
{
    return p < *this;
}

Product &Product::operator=(const Product &p)
{
    if (this == &p)
        return *this;
    this->id = p.id;
    this->stock = p.stock;
    this->price = p.price;
    this->name = p.name;
    this->description = p.description;
    return *this;
}

Product Product::operator+(const Product &p) const
{
    Product result = *this;
    result.price = this->price + p.price;
    result.stock = this->stock + p.stock;
    result.description = this->description + " | " + p.description;
    return result;
}

std::ostream &operator<<(std::ostream &out, const Product &p)
{
    auto truncate = [](const std::string &s, std::size_t w) -> std::string {
        if (s.size() <= w) return s;
        if (w <= 3) return s.substr(0, w);
        return s.substr(0, w - 3) + "...";
    };

    const int wId = 6;
    const int wTipo = 14;
    const int wNombre = 22;
    const int wPrecio = 10;
    const int wStock = 8;
    const int wDesc = 36;

    const int wMarca = 14;
    const int wModelo = 12;
    const int wNombreE = 20;
    const int wGarantia = 10;
    const int wSpecs = 28;

    const int wAutor = 18;
    const int wTitulo = 20;
    const int wEditorial = 16;
    const int wIsbn = 16;
    const int wPag = 8;

    const Electronic* e = dynamic_cast<const Electronic*>(&p);
    if (e)
    {
        std::ostringstream oss; oss << std::fixed << std::setprecision(2) << e->getPrice();
        out << std::left << std::setw(wId) << e->getId()
            << std::setw(wMarca) << truncate(e->getBrand(), wMarca - 1)
            << std::setw(wModelo) << truncate(e->getModel(), wModelo - 1)
            << std::setw(wNombreE) << truncate(e->getName(), wNombreE - 1)
            << std::setw(wPrecio) << oss.str()
            << std::setw(wStock) << e->getStock()
            << std::setw(wGarantia) << e->getWarrantyMonths()
            << std::setw(wSpecs) << truncate(e->getSpecifications(), wSpecs - 1);
        return out;
    }

    const Book* b = dynamic_cast<const Book*>(&p);
    if (b)
    {
        std::ostringstream oss; oss << std::fixed << std::setprecision(2) << b->getPrice();
        out << std::left << std::setw(wId) << b->getId()
            << std::setw(wAutor) << truncate(b->getAuthor(), wAutor - 1)
            << std::setw(wTitulo) << truncate(b->getName(), wTitulo - 1)
            << std::setw(wEditorial) << truncate(b->getPublisher(), wEditorial - 1)
            << std::setw(wIsbn) << truncate(b->getIsbn(), wIsbn - 1)
            << std::setw(wPag) << b->getPages()
            << std::setw(wPrecio) << oss.str()
            << std::setw(wStock) << b->getStock();
        return out;
    }

    std::ostringstream oss; oss << std::fixed << std::setprecision(2) << p.getPrice();
    out << std::left << std::setw(wId) << p.getId()
        << std::setw(wTipo) << "Producto"
        << std::setw(wNombre) << truncate(p.getName(), wNombre - 1)
        << std::setw(wPrecio) << oss.str()
        << std::setw(wStock) << p.getStock()
        << std::setw(wDesc) << truncate(p.getDescription(), wDesc - 1);
    return out;
}

std::istream &operator>>(std::istream &in, Product &p)
{
    int id;
    if (!(in >> id))
        return in;
    p.id = id;
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string name;
    std::getline(in, name);
    p.name = name;

    float price;
    if (!(in >> price))
        return in;
    p.price = price;
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string description;
    std::getline(in, description);
    p.description = description;

    int stock;
    if (in >> stock)
    {
        p.stock = stock;
    }

    return in;
}
