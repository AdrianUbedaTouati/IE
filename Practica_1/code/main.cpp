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
    bool kCasosEspeciales = true, kpasarAminusculas = false;

    list<string> lt1, lt2;

    Tokenizador a("-#:/.", true, false);
    list<string> tokens;

    a.Tokenizar("http:", tokens);
    imprimirListaSTL(tokens);

    a.Tokenizar("http:////ab/", tokens);
    imprimirListaSTL(tokens);

    a.Tokenizar("http:////ab.", tokens);
    imprimirListaSTL(tokens);



}
