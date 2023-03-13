#include "../include/tokenizador.h"

string Tokenizador::FusionarDelimitadores(const string &str1, const string &str2) const {
    string aux(str1);
    for (size_t i = 0; i < str2.size(); ++i) {
        if (aux.find(str2[i]) == -1) {
            aux += str2[i];
        }
    }

    return aux;
}


void Tokenizador::declararEquivalentes(){
    size_t i;
    for(i=0; i<=255; i++)   {equivalenteLetra[i] = i;};
    for (i=65; i<=90; i++)  {equivalenteLetra[i+32];};    //pasar Mays a Mins
    for(i = 224; i<=229; i++) {equivalenteLetra[i] = 97;};    //arreglar as
    for(i = 192; i<=197; i++) {equivalenteLetra[i] = 97;};    //arreglar As
    for(i = 232; i<=235; i++) {equivalenteLetra[i] = 101;};   //arreglar es
    for(i = 200; i<=203; i++) {equivalenteLetra[i] = 101;};   //arreglar Es
    for(i = 236; i<=239; i++) {equivalenteLetra[i] = 105;};   //arreglar is
    for(i = 204; i<=207; i++) {equivalenteLetra[i] = 105;};   //arreglar Is
    for(i = 242; i<=246; i++) {equivalenteLetra[i] = 111;};   //arreglar os
    for(i = 210; i<=212; i++) {equivalenteLetra[i] = 111;};   //arreglar Os
    for(i = 249; i<=252; i++) {equivalenteLetra[i] = 117;};   //arreglar us
    for(i = 217; i<=220; i++) {equivalenteLetra[i] = 117;};   //arreglar Us
    
    equivalenteLetra[209] = 241;    //Ñ a ñ
    equivalenteLetra[253] = 121;    //ý a y
    equivalenteLetra[255] = 121;    //ÿ a y
    equivalenteLetra[221] = 121;    //Ý a y
    equivalenteLetra[199] = 99;     //Ç a c
    equivalenteLetra[231] = 99;     //ç a c
}


string Tokenizador::NormalizarTexto(const string &str) const {
    string aux(str);
    for(size_t i = 0; i < str.size(); ++i) {
        aux[i] = equivalenteLetra[str[i]];
    }
    return aux;
}

Tokenizador::Tokenizador(const string& delimitadoresPalabra, const bool& kcasosEspeciales, const bool& minuscSinAcentos) {
    pasarAminuscSinAcentos = minuscSinAcentos;
    casosEspeciales = kcasosEspeciales;
    delimiters = FusionarDelimitadores("", delimitadoresPalabra);
    declararEquivalentes();
}

Tokenizador::Tokenizador(const Tokenizador &tokenizador) {
    pasarAminuscSinAcentos = tokenizador.pasarAminuscSinAcentos;
    casosEspeciales = tokenizador.casosEspeciales;
    delimiters = tokenizador.delimiters;
    declararEquivalentes();
}

Tokenizador::Tokenizador() {
    pasarAminuscSinAcentos = true;
    casosEspeciales = true;
    delimiters = ",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t\n\r@";
    declararEquivalentes();
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

void Tokenizador::Tokenizar (const string& strP, list<string>& tokens) const {
    for(int i=0; i<256; i++){
        cout<< equivalenteLetra[i] <<endl;
    }
    string str;
    if(pasarAminuscSinAcentos){
        str = NormalizarTexto(strP);
    }
    else{
        str = strP;
    }

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
    /* Cambia la variable privada pasarAminuscSinAcentos. Atenci�n al
    formato de codificaci�n del corpus (comando file de Linux). Para la
    correcci�n de la pr�ctica se utilizar� el formato actual (ISO-8859).*/

bool Tokenizador::PasarAminuscSinAcentos () {
    return pasarAminuscSinAcentos;
}

ostream &operator <<  (ostream& os, const Tokenizador& t) {
    os << "DELIMITADORES: " << t.delimiters 
       << " TRATA CASOS ESPECIALES: " << t.casosEspeciales
       << " PASAR A MINUSCULAS Y SIN ACENTOS: " << t.pasarAminuscSinAcentos;

    return os;
}