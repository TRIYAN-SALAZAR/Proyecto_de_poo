#ifndef UTILITIES_H_INCLUDED
#define UTILITIES_H_INCLUDED

#include <limits>
#include <iostream>
#include <cstdlib>
#include <cstdio>

// (Nota: comentario decorativo movido al final del archivo)

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
