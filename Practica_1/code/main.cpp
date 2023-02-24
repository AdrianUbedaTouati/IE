#include "Tokenizador.h"

int main() {
    string pepa = "pepazo2000awdawdawf421ad3";
    cout<<pepa<<endl;
    Tokenizador pepe(pepa, false, false);
    cout<<pepe.DelimitadoresPalabra();
    return 0;
}
