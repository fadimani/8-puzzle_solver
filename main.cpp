#include <iostream>
using namespace std;

#include "graphemat.h"
#include "liregraphe.h"
#include "Astar_taquin.h"

typedef GrapheMat Graphe;

void menu();
void mainMenu();
void printmainMenu();
int choice1 = 0;


int main() {

    menu();

    return 0;
}

void menu(){
    printmainMenu();
    do {

        mainMenu();

        switch(choice1) {

            case 1: {
                NomSom etatInitial = "813945276";
                bool found = A_star(etatInitial,100);
                if (!found) ::printf("no solution at depth 100");

            } break;
            case 2: {
                NomSom etatInitial = "813945276";
                IDA_star(etatInitial,100);

            } break;
            case 3: {
                printf ("enter a valid starting position (ex:283164795) : ");
                NomSom somD; scanf  ("%s", somD);
                bool found = A_star(somD,100);
                if (!found) ::printf("no solution at depth 100");
            } break;
            case 4: {
                printf ("enter a valid starting position (ex:283164795) : ");
                NomSom somD; scanf  ("%s", somD);
                IDA_star(somD,100);

            } break;






            case 99:{
                printmainMenu();
            }break;
            default:{
                cout << "\nOperation doesn't exist, press 99 for menu ";
            }break;
        }

    } while(choice1 != 0);
}

void mainMenu(void) {
    cout << "\n\nPlease choose (99 to print the menu, 0 to exit) : ";
    cin >> choice1;
}
void printmainMenu(void){


    std::cout << R"(
             .d8888b.         8888888b.                             888
            d88P  Y88b        888   Y88b                            888
            Y88b. d88P        888    888                            888
             "Y88888"         888   d88P 888  888 88888888 88888888 888  .d88b.
            .d8P""Y8b.        8888888P"  888  888    d88P     d88P  888 d8P  Y8b
            888    888 888888 888        888  888   d88P     d88P   888 88888888
            Y88b  d88P        888        Y88b 888  d88P     d88P    888 Y8b.
             "Y8888P"         888         "Y88888 88888888 88888888 888  "Y8888
)" << '\n';


    cout << "                           |8       1       3|            |1       2       3|\n";
    cout << "1  - find the solution     |9       4       5|    ===>    |8       9       4|  using A star\n";
    cout << "                           |2       7       6|            |7       6       5|\n\n";

    cout << "                           |8       1       3|            |1       2       3|\n";
    cout << "2  - find the solution     |9       4       5|    ===>    |8       9       4|  using IDA star\n";
    cout << "                           |2       7       6|            |7       6       5|\n\n";


    cout << "                           |x       x       x|            |1       2       3|\n";
    cout << "3  - find the solution     |x       x       x|    ===>    |8       9       4|  using A star\n";
    cout << " (input starter position)  |x       x       x|            |7       6       5|\n\n";

    cout << "                           |x       x       x|            |1       2       3|\n";
    cout << "4  - find the solution     |x       x       x|    ===>    |8       9       4|  using IDA star\n";
    cout << " (input starter position)  |x       x       x|            |7       6       5|\n";

}
