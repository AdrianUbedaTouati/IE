#include "../include/tokenizador.h"

#include <iostream>
#include <ostream>
#include <string>

using namespace std;

int main() {
    string pepa = "pepazo2000awdawdawf421ad3";
    cout<<pepa<<endl;
    Tokenizador pepe(pepa, false, false);
    cout<<pepe.DelimitadoresPalabra();
    return 0;
}
