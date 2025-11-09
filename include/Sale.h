#ifndef SALE_H
#define SALE_H

#include <string>

struct Sale {
    int saleId;
    int userCode;
    std::string userName;
    std::string productType;
    std::string productName;
    int units;
    float totalPrice;

    Sale() : saleId(0), userCode(0), userName(""), productType(""), productName(""), units(0), totalPrice(0.0f) {}
    Sale(int sid, int ucode, const std::string &uname, const std::string &ptype, const std::string &pname, int units, float total)
        : saleId(sid), userCode(ucode), userName(uname), productType(ptype), productName(pname), units(units), totalPrice(total) {}
};

#endif // SALE_H
