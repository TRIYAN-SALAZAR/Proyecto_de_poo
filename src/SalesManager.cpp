#include "SalesManager.h"

SalesManager::SalesManager()
    : sales(), counter(0)
{
}

SalesManager::~SalesManager() = default;

int SalesManager::addSale(const Sale &s)
{
    int id = ++counter;
    Sale copy = s;
    copy.saleId = id;
    sales.push_back(copy);
    return id;
}

Sale* SalesManager::findById(int saleId)
{
    for (auto &s : sales) {
        if (s.saleId == saleId) return &s;
    }
    return nullptr;
}

std::vector<Sale> SalesManager::findByUserCode(int userCode) const
{
    std::vector<Sale> res;
    for (const auto &s : sales) {
        if (s.userCode == userCode) res.push_back(s);
    }
    return res;
}
