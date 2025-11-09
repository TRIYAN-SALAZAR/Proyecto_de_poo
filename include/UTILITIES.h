#ifndef UTILITIES_H_INCLUDED
#define UTILITIES_H_INCLUDED

#include <limits>
#include <iostream>
#include <cstdlib>
#include <cstdio>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

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

inline std::string readPassword()
{
    std::string pwd;
#ifdef _WIN32
    int ch;
    while ((ch = _getch()) != '\r' && ch != '\n' && ch != EOF) {
        if (ch == 8) { // backspace
            if (!pwd.empty()) pwd.pop_back();
        } else {
            pwd.push_back((char)ch);
        }
    }
#else
    struct termios oldt, newt;
    if (tcgetattr(STDIN_FILENO, &oldt) == 0) {
        newt = oldt;
        newt.c_lflag &= ~ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
            if (ch == 127 || ch == '\b') {
                if (!pwd.empty()) pwd.pop_back();
            } else {
                pwd.push_back((char)ch);
            }
        }
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    } else {
        // Fallback
        std::getline(std::cin, pwd);
    }
#endif
    std::cout << std::endl;
    return pwd;
}

#endif // UTILITIES_H_INCLUDED
