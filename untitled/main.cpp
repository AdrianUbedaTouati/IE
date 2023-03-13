#include "../include/tokenizador.h"

#include <iostream>
#include <ostream>
#include <string>

using namespace std;

int main (int argc, char **argv) {
    bool a =false;
    bool b = false;
    list<string> tokens;
    Tokenizador pepe = Tokenizador();
    pepe.Tokenizar("HÓla/UWû", tokens);
    auto l_front = tokens.begin();
    cout<<*l_front <<endl;
    advance(l_front, 1);

    cout << *l_front << endl;
    
    return 0;
}
