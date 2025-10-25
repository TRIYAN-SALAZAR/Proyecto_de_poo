#include "Electronic.h"
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
