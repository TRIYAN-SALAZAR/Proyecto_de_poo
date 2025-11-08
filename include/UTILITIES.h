#ifndef UTILITIES_H_INCLUDED
#define UTILITIES_H_INCLUDED

#include <limits>
#include <iostream>
#include <cstdlib>

inline void clearInput()
{
    // Descarta hasta el siguiente '\n' o hasta el máximo tamaño del stream
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

inline void clearScreen()
{
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

inline void waitForEnter()
{
    std::cout << "\nPresione ENTER para continuar...";
    std::string _tmp;
    std::getline(std::cin, _tmp);
}

#endif // UTILITIES_H_INCLUDED
