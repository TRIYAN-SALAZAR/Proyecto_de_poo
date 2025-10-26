#include "Electronic.h"
#include <iomanip>
#include <iostream>

Electronic::Electronic()
    : Product(), brand(""), model(""), warrantyMonths(0), specifications("")
{
}

Electronic::Electronic(int id, const std::string &name, float price, const std::string &description, int stock,
                       const std::string &brand, const std::string &model, int warrantyMonths, const std::string &specifications)
    : Product(id, stock, price, name, description), brand(brand), model(model), warrantyMonths(warrantyMonths), specifications(specifications)
{
}

std::string Electronic::getBrand() const { return brand; }
void Electronic::setBrand(const std::string &brand) { this->brand = brand; }

std::string Electronic::getModel() const { return model; }
void Electronic::setModel(const std::string &model) { this->model = model; }

int Electronic::getWarrantyMonths() const { return warrantyMonths; }
void Electronic::setWarrantyMonths(int warrantyMonths) { this->warrantyMonths = warrantyMonths; }

std::string Electronic::getSpecifications() const { return specifications; }
void Electronic::setSpecifications(const std::string &specifications) { this->specifications = specifications; }

void Electronic::showInfo() const
{
    std::cout << "Producto electronico ID: " << getId() << "\n"
              << "Nombre: " << getName() << "\n"
              << "Marca: " << brand << "\n"
              << "Modelo: " << model << "\n"
              << "Descripcion: " << getDescription() << "\n"
              << "Especificaciones: " << specifications << "\n"
              << "Precio: " << getPrice() << "\n"
              << "Garantia meses: " << warrantyMonths << "\n"
              << "Stock: " << getStock() << std::endl;
}

float Electronic::calculateFinalPrice() const
{
    const float ELECTRONIC_TAX = 0.16f;
    return getPrice() * (1.0f + ELECTRONIC_TAX);
}

void Electronic::printRow(std::ostream &out) const
{
    const int wId = 6;
    const int wMarca = 14;
    const int wModelo = 12;
    const int wNombre = 20;
    const int wPrecio = 10;
    const int wStock = 8;
    const int wGarantia = 10;
    const int wSpecs = 28;

    auto truncate = [](const std::string &s, std::size_t w) -> std::string {
        if (s.size() <= w) return s;
        if (w <= 3) return s.substr(0, w);
        return s.substr(0, w - 3) + "...";
    };

    std::ostringstream oss; oss << std::fixed << std::setprecision(2) << getPrice();
    out << std::left << std::setw(wId) << getId()
        << std::setw(wMarca) << truncate(brand, wMarca - 1)
        << std::setw(wModelo) << truncate(model, wModelo - 1)
        << std::setw(wNombre) << truncate(getName(), wNombre - 1)
        << std::setw(wPrecio) << oss.str()
        << std::setw(wStock) << getStock()
        << std::setw(wGarantia) << warrantyMonths
        << std::setw(wSpecs) << truncate(specifications, wSpecs - 1);
}
