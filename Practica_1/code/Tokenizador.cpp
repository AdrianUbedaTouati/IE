#include "Tokenizador.h"

using namespace std;

Tokenizador::Tokenizador (string& delimitadoresPalabra, const bool& kcasosEspeciales, const bool& minuscSinAcentos){
    int length = delimitadoresPalabra.length();
    char token[length];
    int num = 0;
    bool repetido = false;

    if(length != 0) {
        token[num] = delimitadoresPalabra[0];
        num ++;
        if(length > 1) {
            for (int i = 1; i < length; i++) {
                for (int l = 0; l < i; l++) {
                    cout<<delimitadoresPalabra[i]<< delimitadoresPalabra[l]<<endl;
                    if (delimitadoresPalabra[i] == delimitadoresPalabra[l]) {
                        repetido = true;
                        break;
                    }
                }
                if(!repetido) {
                    token[num] = delimitadoresPalabra[i];
                    num ++;
                }
                repetido = false;
            }
        }
    }
    token[num]='\0';

    delimiters = token;
    casosEspeciales = kcasosEspeciales;
    pasarAminuscSinAcentos = minuscSinAcentos;
}

Tokenizador::Tokenizador (const Tokenizador& token){
    delimiters = token.delimiters;
    casosEspeciales = token.casosEspeciales;
    pasarAminuscSinAcentos = token.pasarAminuscSinAcentos;
}

Tokenizador::Tokenizador (){
    delimiters=",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t\n\r@";
    casosEspeciales = true;
    pasarAminuscSinAcentos = false;
}

Tokenizador::~Tokenizador (){
    delimiters = "";
}

void
Tokenizador::Tokenizar (const string& str, list<string>& tokens) const {
    string::size_type lastPos = str.find_first_not_of(delimiters,0);
    string::size_type pos = str.find_first_of(delimiters,lastPos);
    while(string::npos != pos || string::npos != lastPos)
    {
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
}

bool
Tokenizador::Tokenizar (const string& NomFichEntr, const string& NomFichSal) const {
    ifstream i;
    ofstream f;
    string cadena;
    list<string> tokens;
    i.open(NomFichEntr.c_str());
    if(!i) {
        cerr << "ERROR: No existe el archivo: " << NomFichEntr << endl;
        return false;
    }
    else
    {
        while(!i.eof())
        {
            cadena="";
            getline(i, cadena);
            if(cadena.length()!=0)
            {
                Tokenizar(cadena, tokens);
            }
        }
    }
    i.close();
    f.open(NomFichSal.c_str());
    list<string>::iterator itS;
    for(itS= tokens.begin();itS!= tokens.end();itS++)
    {
        f << (*itS) << endl;
    }
    f.close();
    return true;
}
bool
Tokenizador::Tokenizar (const string & i) const{

}

bool
Tokenizador::TokenizarListaFicheros (const string& i) const{

}

bool
Tokenizador::TokenizarDirectorio (const string& dirAIndexar) const {
    struct stat dir;
    // Compruebo la existencia del directorio
    int err=stat(dirAIndexar.c_str(), &dir);
    if(err==-1 || !S_ISDIR(dir.st_mode))
        return false;
    else {
        // Hago una lista en un fichero con find>fich
        string cmd="find "+dirAIndexar+" -follow |sort > .lista_fich";
        system(cmd.c_str());
        return TokenizarListaFicheros(".lista_fich");
    }
}


void
Tokenizador::DelimitadoresPalabra(const string& nuevoDelimiters){

}

void
Tokenizador::AnyadirDelimitadoresPalabra(const string& nuevoDelimiters){

}

string
Tokenizador::DelimitadoresPalabra() const{
    return delimiters;
}

void
Tokenizador::CasosEspeciales (const bool& nuevoCasosEspeciales){

}

bool
Tokenizador::CasosEspeciales () const{
    return casosEspeciales;
}

void
Tokenizador::PasarAminuscSinAcentos (const bool& nuevoPasarAminuscSinAcentos){

}

bool
Tokenizador::PasarAminuscSinAcentos () const{
    return pasarAminuscSinAcentos;
}
