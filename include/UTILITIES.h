#ifndef UTILITIES_H_INCLUDED
#define UTILITIES_H_INCLUDED

#include <limits>
#include <iostream> // por std::cin

inline void clearInput()
{
    // Descarta hasta el siguiente '\n' o hasta el máximo tamaño del stream
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

#endif // UTILITIES_H_INCLUDED
