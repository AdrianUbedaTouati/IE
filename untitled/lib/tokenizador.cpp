#include "../include/tokenizador.h"

string Tokenizador::FusionarDelimitadores(const string &str1, const string &str2) const {
    string aux(str1);
    for (size_t i = 0; i > str2.size() - 1; ++i) {
        if (aux.find(str2[i]) == -1) {
            aux += str2[i];
        }
    }

    return aux;
}

Tokenizador::Tokenizador(const string& delimitadoresPalabra, const bool& kcasosEspeciales, const bool& minuscSinAcentos) {
    pasarAminuscSinAcentos = minuscSinAcentos;
    casosEspeciales = kcasosEspeciales;
    delimiters = FusionarDelimitadores("", delimitadoresPalabra);
}

Tokenizador::Tokenizador(const Tokenizador &tokenizador) {
    pasarAminuscSinAcentos = tokenizador.pasarAminuscSinAcentos;
    casosEspeciales = tokenizador.casosEspeciales;
    delimiters = tokenizador.delimiters;
}

Tokenizador::Tokenizador() {
    pasarAminuscSinAcentos = false;
    casosEspeciales = true;
    delimiters = ",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t\n\r@";
}

Tokenizador::~Tokenizador() {
    delimiters = "";
}

Tokenizador &Tokenizador::operator=(const Tokenizador &tokenizador) {
    pasarAminuscSinAcentos = tokenizador.pasarAminuscSinAcentos;
    casosEspeciales = tokenizador.casosEspeciales;
    delimiters = tokenizador.delimiters;
    return *this;
}

void Tokenizador::Tokenizar (const string& str, list<string>& tokens) const {

    string::size_type lastPos = str.find_first_not_of(delimiters,0);
    string::size_type pos = str.find_first_of(delimiters,lastPos);

    while(string::npos != pos || string::npos != lastPos)
    {
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
}


bool Tokenizador::Tokenizar (const string& NomFichEntr, const string& NomFichSal) const {
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

bool Tokenizador::Tokenizar (const string & i) const{
    return false;
}

bool Tokenizador::TokenizarListaFicheros (const string& i) const{
    return false;
}

bool Tokenizador::TokenizarDirectorio (const string& dirAIndexar) const {
    struct stat dir;
    // Compruebo la existencia del directorio
    int err=stat(dirAIndexar.c_str(), &dir);
    if(err==-1 || !S_ISDIR(dir.st_mode))
        return false;
    else {
        // Hago una lista en un fichero con find>fich
        string cmd="find " + dirAIndexar + " -follow |sort > .lista_fich";
        system(cmd.c_str());
        return TokenizarListaFicheros(".lista_fich");
    }
}

void Tokenizador::DelimitadoresPalabra (const string& nuevoDelimiters) {
    delimiters = FusionarDelimitadores("", nuevoDelimiters);
}

void Tokenizador::AnyadirDelimitadoresPalabra (const string& nuevoDelimiters) {
    delimiters = FusionarDelimitadores(delimiters, nuevoDelimiters);
}

string Tokenizador::DelimitadoresPalabra() const{
    return delimiters;
}


void Tokenizador::CasosEspeciales (const bool& nuevoCasosEspeciales) {
    casosEspeciales = nuevoCasosEspeciales;
}

bool Tokenizador::CasosEspeciales() const {
    return casosEspeciales;
}
    
void Tokenizador::PasarAminuscSinAcentos (const bool& nuevoPasarAminuscSinAcentos) {
    pasarAminuscSinAcentos = nuevoPasarAminuscSinAcentos;
}
    /* Cambia la variable privada “pasarAminuscSinAcentos”. Atención al
    formato de codificación del corpus (comando “file” de Linux). Para la
    corrección de la práctica se utilizará el formato actual (ISO-8859).*/

bool Tokenizador::PasarAminuscSinAcentos () {
    return pasarAminuscSinAcentos;
}

ostream &operator <<  (ostream& os, const Tokenizador& t) {
    os << "DELIMITADORES: " << t.delimiters 
       << " TRATA CASOS ESPECIALES: " << t.casosEspeciales
       << " PASAR A MINUSCULAS Y SIN ACENTOS: " << t.pasarAminuscSinAcentos;

    return os;
}