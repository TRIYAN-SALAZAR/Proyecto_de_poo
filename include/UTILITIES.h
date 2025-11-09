#ifndef UTILITIES_H_INCLUDED
#define UTILITIES_H_INCLUDED

#include <limits>
#include <iostream>
#include <cstdlib>
#include <cstdio>

// (Nota: comentario decorativo movido al final del archivo)

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

// Key reading abstraction for simple single-key navigation (Windows-only
// implementation here). The function readKey will return a Key value and
// optionally set the output character for printable keys.
enum class Key {
    Up,
    Down,
    Left,
    Right,
    Enter,
    Esc,
    Char,
    Unknown
};

inline Key readKey(char &outChar)
{
#ifdef _WIN32
    outChar = '\0';
    int c = _getch();
    if (c == 0 || c == 0xE0) {
        // Extended key (arrows, function keys)
        int c2 = _getch();
        switch (c2) {
            case 72: return Key::Up;
            case 80: return Key::Down;
            case 75: return Key::Left;
            case 77: return Key::Right;
            default: return Key::Unknown;
        }
    } else {
        if (c == 13) return Key::Enter;
        if (c == 27) return Key::Esc;
        if (c >= 32 && c <= 126) { outChar = (char)c; return Key::Char; }
        return Key::Unknown;
    }
#else
    // POSIX not implemented in this header for now; return Unknown so
    // caller can fall back to numeric input. We'll implement POSIX later.
    outChar = '\0';
    return Key::Unknown;
#endif
}

// Detecta si la consola Windows soporta ANSI (VT sequences) y trata de
// habilitarlo. Devuelve true si las secuencias ANSI pueden usarse.
inline bool supportsAnsi()
{
#ifdef _WIN32
    static int cached = -1;
    if (cached != -1) return cached == 1;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) { cached = 0; return false; }
    DWORD mode = 0;
    if (!GetConsoleMode(hOut, &mode)) { cached = 0; return false; }
    // Try to enable ENABLE_VIRTUAL_TERMINAL_PROCESSING
    DWORD newMode = mode | 0x0004; // ENABLE_VIRTUAL_TERMINAL_PROCESSING
    if (SetConsoleMode(hOut, newMode)) { cached = 1; return true; }
    // Could not enable; keep original mode and return false
    cached = 0;
    return false;
#else
    return true; // POSIX terminals normally support ANSI
#endif
}

// Imprime una línea resaltada. Si ANSI está disponible usa la secuencia
// de inversión; en cmd.exe (sin ANSI) usa SetConsoleTextAttribute para
// invertir atributos visuales y luego restaura el estado.
inline void printHighlightedLine(const std::string &line)
{
#ifdef _WIN32
    if (supportsAnsi()) {
        std::cout << "\x1b[7m" << line << "\x1b[0m" << std::endl;
        return;
    }
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) { std::cout << line << std::endl; return; }
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hOut, &csbi)) { std::cout << line << std::endl; return; }
    WORD orig = csbi.wAttributes;
    // Swap foreground and background nibbles to approximate inverse
    WORD swapped = (WORD)(((orig & 0x0F) << 4) | ((orig & 0xF0) >> 4));
    // Preserve intensity bits where possible
    if (orig & 0x08) swapped |= 0x80; // preserve background intensity to a best-effort
    SetConsoleTextAttribute(hOut, swapped);
    std::cout << line << std::endl;
    SetConsoleTextAttribute(hOut, orig);
#else
    std::cout << "\x1b[7m" << line << "\x1b[0m" << std::endl;
#endif
}

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

/*
                                                                                                                                                                                           
                                                                         dddddddd                                                                                                      
        CCCCCCCCCCCCC                                                    d::::::d  iiii          tttt                                                                                  
     CCC::::::::::::C                                                    d::::::d i::::i      ttt:::t                                                                                  
   CC:::::::::::::::C                                                    d::::::d  iiii       t:::::t                                                                                  
  C:::::CCCCCCCC::::C                                                    d:::::d              t:::::t                                                                                  
 C:::::C       CCCCCCrrrrr   rrrrrrrrr       eeeeeeeeeeee        ddddddddd:::::d iiiiiiittttttt:::::ttttttt       ooooooooooo       ssssssssss                    aaaaaaaaaaaaa        
C:::::C              r::::rrr:::::::::r    ee::::::::::::ee    dd::::::::::::::d i:::::it:::::::::::::::::t     oo:::::::::::oo   ss::::::::::s                   a::::::::::::a       
C:::::C              r:::::::::::::::::r  e::::::eeeee:::::ee d::::::::::::::::d  i::::it:::::::::::::::::t    o:::::::::::::::oss:::::::::::::s                  aaaaaaaaa:::::a      
C:::::C              rr::::::rrrrr::::::re::::::e     e:::::ed:::::::ddddd:::::d  i::::itttttt:::::::tttttt    o:::::ooooo:::::os::::::ssss:::::s                          a::::a      
C:::::C               r:::::r     r:::::re:::::::eeeee::::::ed::::::d    d:::::d  i::::i      t:::::t          o::::o     o::::o s:::::s  ssssss                    aaaaaaa:::::a      
C:::::C               r:::::r     rrrrrrre:::::::::::::::::e d:::::d     d:::::d  i::::i      t:::::t          o::::o     o::::o   s::::::s                       aa::::::::::::a      
C:::::C               r:::::r            e::::::eeeeeeeeeee  d:::::d     d:::::d  i::::i      t:::::t          o::::o     o::::o      s::::::s                   a::::aaaa::::::a      
 C:::::C       CCCCCC r:::::r            e:::::::e           d:::::d     d:::::d  i::::i      t:::::t    tttttto::::o     o::::ossssss   s:::::s                a::::a    a:::::a      
  C:::::CCCCCCCC::::C r:::::r            e::::::::e          d::::::ddddd::::::ddi::::::i     t::::::tttt:::::to:::::ooooo:::::os:::::ssss::::::s               a::::a    a:::::a      
   CC:::::::::::::::C r:::::r             e::::::::eeeeeeee   d:::::::::::::::::di::::::i     tt::::::::::::::to:::::::::::::::os::::::::::::::s                a:::::aaaa::::::a      
     CCC::::::::::::C r:::::r              ee:::::::::::::e    d:::::::::ddd::::di::::::i       tt:::::::::::tt oo:::::::::::oo  s:::::::::::ss                  a::::::::::aa:::a     
        CCCCCCCCCCCCC rrrrrrr                eeeeeeeeeeeeee     ddddddddd   dddddiiiiiiii         ttttttttttt     ooooooooooo     sssssssssss                     aaaaaaaaaa  aaaa     
                                                                                                                                                                                       
                                                                                                                                                                                                                                                                                                                                                       
                                                                                                                                                                                       
                                                                                                                                                                                       
                                                                                                                                                                                       
                            CCCCCCCCCCCCChhhhhhh                                       tttt                 GGGGGGGGGGGGGPPPPPPPPPPPPPPPPP   TTTTTTTTTTTTTTTTTTTTTTT                   
                         CCC::::::::::::Ch:::::h                                    ttt:::t              GGG::::::::::::GP::::::::::::::::P  T:::::::::::::::::::::T                   
                       CC:::::::::::::::Ch:::::h                                    t:::::t            GG:::::::::::::::GP::::::PPPPPP:::::P T:::::::::::::::::::::T                   
                      C:::::CCCCCCCC::::Ch:::::h                                    t:::::t           G:::::GGGGGGGG::::GPP:::::P     P:::::PT:::::TT:::::::TT:::::T                   
                     C:::::C       CCCCCC h::::h hhhhh         aaaaaaaaaaaaa  ttttttt:::::ttttttt    G:::::G       GGGGGG  P::::P     P:::::PTTTTTT  T:::::T  TTTTTT                   
                    C:::::C               h::::hh:::::hhh      a::::::::::::a t:::::::::::::::::t   G:::::G                P::::P     P:::::P        T:::::T                           
                    C:::::C               h::::::::::::::hh    aaaaaaaaa:::::at:::::::::::::::::t   G:::::G                P::::PPPPPP:::::P         T:::::T                           
                    C:::::C               h:::::::hhh::::::h            a::::atttttt:::::::tttttt   G:::::G    GGGGGGGGGG  P:::::::::::::PP          T:::::T                           
                    C:::::C               h::::::h   h::::::h    aaaaaaa:::::a      t:::::t         G:::::G    G::::::::G  P::::PPPPPPPPP            T:::::T                           
                    C:::::C               h:::::h     h:::::h  aa::::::::::::a      t:::::t         G:::::G    GGGGG::::G  P::::P                    T:::::T                           
                    C:::::C               h:::::h     h:::::h a::::aaaa::::::a      t:::::t         G:::::G        G::::G  P::::P                    T:::::T                           
                     C:::::C       CCCCCC h:::::h     h:::::ha::::a    a:::::a      t:::::t    ttttttG:::::G       G::::G  P::::P                    T:::::T                           
                      C:::::CCCCCCCC::::C h:::::h     h:::::ha::::a    a:::::a      t::::::tttt:::::t G:::::GGGGGGGG::::GPP::::::PP                TT:::::::TT                         
                       CC:::::::::::::::C h:::::h     h:::::ha:::::aaaa::::::a      tt::::::::::::::t  GG:::::::::::::::GP::::::::P                T:::::::::T                         
                         CCC::::::::::::C h:::::h     h:::::h a::::::::::aa:::a       tt:::::::::::tt    GGG::::::GGG:::GP::::::::P                T:::::::::T                         
                            CCCCCCCCCCCCC hhhhhhh     hhhhhhh  aaaaaaaaaa  aaaa         ttttttttttt         GGGGGG   GGGGPPPPPPPPPP                TTTTTTTTTTT                         
                                                                                                                                                                                       
                                                                                                                                                                                       
                                                                                                                                                                                       
                                                                                                                                                                                       
                                                                                                                                                                                       
                                                                                                                                                                                       
                                                                                                                                                                                       
*/

#endif // UTILITIES_H_INCLUDED
