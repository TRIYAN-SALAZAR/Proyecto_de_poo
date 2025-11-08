#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <iostream>

class Product
{
    protected:
        int id;
        int stock;
        float price;
        std::string name;
        std::string description;

    public:
    Product();
    Product(int id, int stock, float price, const std::string& name, const std::string& description);

    int getId() const;
    void setId(int id);

    std::string getName() const;
    void setName(const std::string& name);

    float getPrice() const;
    void setPrice(float price);

    std::string getDescription() const;
    void setDescription(const std::string& description);

    int getStock() const;
    void setStock(int stock);

    virtual void showInfo() const;
    virtual float calculateFinalPrice() const;
    virtual void printRow(std::ostream& out) const;
    virtual ~Product();

    bool operator==(const Product& p) const;
    bool operator!=(const Product& p) const;
    bool operator<(const Product& p) const;
    bool operator>(const Product& p) const;

    Product& operator=(const Product& p);
    Product operator+(const Product& p) const;

    friend std::ostream& operator<<(std::ostream& out, const Product& p);
    friend std::istream& operator>>(std::istream& input, Product& p);
};

#endif // PRODUCT_H
