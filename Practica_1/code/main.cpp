#include "Tokenizador.h"

void imprimirListaSTL(const list<string>& cadena)
{
    list<string>::const_iterator itCadena;
    for(itCadena=cadena.begin();itCadena!=cadena.end();itCadena++)
    {
        cout << (*itCadena) << ", ";
    }
    cout << endl;
}

int main() {
    //string s1 = "á";
    //cout << s1.size() << endl;

    string pepa = "@.-/";

    Tokenizador pepe(pepa, true, false);
    cout<<pepe.DelimitadoresPalabra()<<endl;

    list<string> tokens;

    string s = "hola soy pepepe 123 1,2 1, 23,4 eppe@gmail.com http:gola";

    pepe.Tokenizar(s,tokens);

    imprimirListaSTL(tokens);


    return 0;
}
