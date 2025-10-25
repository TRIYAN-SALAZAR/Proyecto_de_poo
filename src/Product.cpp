#include "Product.h"
#include <iostream>
#include <limits>

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
    out << "Producto[" << p.id << "] "
        << "Nombre: " << p.name << ", "
        << "Precio: " << p.price << ", "
        << "Stock: " << p.stock << ", "
        << "Descripcion: " << p.description;
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
