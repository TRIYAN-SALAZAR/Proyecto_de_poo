#include <iostream>
#include <limits>
#include <iomanip>
#include "Product.h"
#include "UTILITIES.h"

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
    return !(this->id == p.id);
}

bool Product::operator<(const Product &p) const
{
    if (this->price < p.price) return true;
    return false;
}

bool Product::operator>(const Product &p) const
{
    if (this->price > p.price) return true;
    return false;
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
    return result;
}

std::ostream &operator<<(std::ostream &out, const Product &p)
{
    p.printRow(out);
    return out;
}

void Product::printRow(std::ostream &out) const
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

    std::ostringstream oss; oss << std::fixed << std::setprecision(2) << getPrice();
    out << std::left << std::setw(wId) << getId()
        << std::setw(wTipo) << "Producto"
        << std::setw(wNombre) << truncate(getName(), wNombre - 1)
        << std::setw(wPrecio) << oss.str()
        << std::setw(wStock) << getStock()
        << std::setw(wDesc) << truncate(getDescription(), wDesc - 1);
}

std::istream &operator>>(std::istream &input, Product &p)
{
    std::cout << std::endl;
    std::cout << "Ingrese el nombre: ";
    std::getline(input, p.name);
    clearInput();

    std::cout << "Ingrese la descripcion: ";
    std::getline(input, p.description);
    clearInput();

    std::cout << "Ingrese el stock: ";
    input >> p.stock;
    
    std::cout << "Ingrese el precio: ";
    input >> p.price;
    std::cout << std::endl;

    return input;
}
