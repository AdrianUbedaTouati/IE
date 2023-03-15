#include "../include/tokenizador.h"

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

    Tokenizador a("@.&", true, false);
    list<string> tokens;
    a.DelimitadoresPalabra("/ &_:/.?&-=#@");
    string s = "p0 http://intime.dlsi.ua.es:8080/dossierct/index.jsp?lang=es&status=probable&date=22-01-2013&newspaper=catedraTelefonicaUA@iuii.ua.es p1 p2";

    a.Tokenizar("p0 http://intime.dlsi.ua.es:8080/dossierct/index.jsp?lang=es&status=probable&date=22-01-2013&newspaper=catedraTelefonicaUA@iuii.ua.es p1 p2", tokens);
    imprimirListaSTL(tokens);

}
