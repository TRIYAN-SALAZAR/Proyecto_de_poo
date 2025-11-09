#ifndef TABLEFORMAT_H_INCLUDED
#define TABLEFORMAT_H_INCLUDED

#include <sstream>
#include <iomanip>
#include <string>
#include <initializer_list>
#include <ostream>

namespace TableFormat {
    struct ProductWidths {
        static constexpr int Id = 6;
        static constexpr int Tipo = 14;
        static constexpr int Nombre = 22;
        static constexpr int Precio = 10;
        static constexpr int Stock = 8;
        static constexpr int Desc = 36;
    };

    struct ElectronicWidths {
        static constexpr int Id = 6;
        static constexpr int Marca = 14;
        static constexpr int Modelo = 12;
        static constexpr int Nombre = 20;
        static constexpr int Precio = 10;
        static constexpr int Stock = 8;
        static constexpr int Garantia = 10;
        static constexpr int Specs = 28;
    };

    struct BookWidths {
        static constexpr int Id = 6;
        static constexpr int Autor = 18;
        static constexpr int Titulo = 20;
        static constexpr int Editorial = 16;
        static constexpr int Isbn = 16;
        static constexpr int Pag = 8;
        static constexpr int Precio = 10;
        static constexpr int Stock = 8;
    };

    struct UserWidths {
        static constexpr int Id = 6;
        static constexpr int Name = 24;
        static constexpr int Role = 16;
    };

    struct SalesWidths {
        static constexpr int Id = 6;
        static constexpr int UCode = 10;
        static constexpr int UName = 16;
        static constexpr int PType = 12;
        static constexpr int PName = 22;
        static constexpr int Units = 8;
        static constexpr int Total = 12;
    };

    inline std::string formatPrice(float v) {
        std::ostringstream oss; oss << std::fixed << std::setprecision(2) << v; return oss.str();
    }

    inline void printSeparator(std::ostream &out, std::initializer_list<int> widths) {
        int total = 0; for (int w : widths) total += w; out << std::string(total, '-') << "\n";
    }
}


#endif // TABLEFORMAT_H_INCLUDED
