#ifndef SALESMANAGER_H
#define SALESMANAGER_H

#include <vector>
#include "Sale.h"

class SalesManager {
private:
    std::vector<Sale> sales;
    int counter;
public:
    SalesManager();
    ~SalesManager();

    int addSale(const Sale &s); // returns assigned sale id
    Sale* findById(int saleId);
    std::vector<Sale> findByUserCode(int userCode) const;
    const std::vector<Sale>& allSales() const { return sales; }
};

#endif // SALESMANAGER_H
