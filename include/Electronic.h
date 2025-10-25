#ifndef ELECTRONIC_H
#define ELECTRONIC_H

#include <Product.h>
#include <string>

class Electronic : public Product
{
    private:
        std::string brand;
        std::string model;
        int warrantyMonths;
        std::string specifications;

    public:
        Electronic();
        Electronic(int id, const std::string& name, float price, const std::string& description, int stock,
                   const std::string& brand, const std::string& model, int warrantyMonths, const std::string& specifications);

        std::string getBrand() const;
        void setBrand(const std::string& brand);

        std::string getModel() const;
        void setModel(const std::string& model);

        int getWarrantyMonths() const;
        void setWarrantyMonths(int warrantyMonths);

        std::string getSpecifications() const;
        void setSpecifications(const std::string& specifications);

        void showInfo() const override;
        float calculateFinalPrice() const override;
};

#endif // ELECTRONIC_H
