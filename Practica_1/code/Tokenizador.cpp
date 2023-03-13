#include "Tokenizador.h"

using namespace std;

Tokenizador::Tokenizador (const char* delimitadoresPalabra, const bool& kcasosEspeciales, const bool& minuscSinAcentos) {
    string aux= delimitadoresPalabra;
    delimiters = PreparacionDelimitadores(aux);
    casosEspeciales = kcasosEspeciales;
    pasarAminuscSinAcentos = minuscSinAcentos;

}

Tokenizador::Tokenizador (const Tokenizador& tokenizador){
    delimiters = tokenizador.delimiters;
    casosEspeciales = tokenizador.casosEspeciales;
    pasarAminuscSinAcentos = tokenizador.pasarAminuscSinAcentos;
}

Tokenizador::Tokenizador (){
    delimiters=",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t\n\r@";
    casosEspeciales = true;
    pasarAminuscSinAcentos = false;
}

Tokenizador::~Tokenizador (){
    delimiters = "";
}

Tokenizador
&Tokenizador::operator=(const Tokenizador& tokenizador) {
    delimiters = tokenizador.delimiters;
    casosEspeciales = tokenizador.casosEspeciales;
    pasarAminuscSinAcentos = tokenizador.pasarAminuscSinAcentos;
    return *this;
}

ostream& operator<<(ostream& os, const Tokenizador& tokenizador) {
    os << "DELIMITADORES: " << tokenizador.delimiters << " TRATA CASOS ESPECIALES: " <<
    tokenizador.casosEspeciales << " PASAR A MINUSCULAS Y SIN ACENTOS: " << tokenizador.pasarAminuscSinAcentos;
    return os;
}

void
Tokenizador::Tokenizar (const char* str1, list<string>& tokens) {
    string str = str1;
    tokens.clear();
    string delimitadores = delimiters;

    if(pasarAminuscSinAcentos){
        PasarAminuscSinAcentosFun(str);
    }

    if(casosEspeciales) {
        if (delimiters.find(" ") == string::npos) {
            delimitadores += " ";
        }
    }
    AuxTokenizar(str,tokens,delimitadores);
}

void
Tokenizador::AuxTokenizar (const string& str, list<string>& tokens,string delimitadoresPalabra) {
    bool primerToken = true;
    string token;
    string::size_type primerCaracter = str.find_first_not_of(delimitadoresPalabra,0);
    string::size_type posDelimitador = str.find_first_of(delimitadoresPalabra,primerCaracter);
    while(posDelimitador != string::npos || primerCaracter != string::npos) {
        if (!casosEspeciales) {
            tokens.push_back(str.substr(primerCaracter, posDelimitador - primerCaracter));
        } else {

            if (primerToken) {
                token = str.substr(primerCaracter, posDelimitador - primerCaracter + 1);
            } else {
                token = str.substr(primerCaracter - 1, posDelimitador - primerCaracter + 2);
            }

            if (isUrl && primerToken && (token.find("http:") == 0 || token.find("https:") == 0 || token.find("ftp:") == 0)
                || (delimitadoresPalabra.find(token[0]) != string::npos && (token.find("http:") == 1
                || token.find("https:") == 1 ||token.find("ftp:") == 1))) {
                TratarURL(str, delimitadoresPalabra, posDelimitador);
            }
            else if (IsDecimal(token)){
                if (token[0] == '.' || token[0] == ',') {
                    string aux = token;
                    aux = "0" + aux;
                    tokens.push_back(aux);
                    primerCaracter = str.find_first_not_of(delimitadoresPalabra, posDelimitador);
                    posDelimitador = str.find_first_of(delimitadoresPalabra, primerCaracter);
                    continue;
                }
            }else if (token[token.size()-1] == '@') {
                TratarEmail(str,delimitadoresPalabra, posDelimitador);
                //cout << "Soy el token" << token << " y soy un mail" << endl;
            } else if (token[token.size()-1] == '.') {
                TratarAcronimo(str,delimitadoresPalabra, posDelimitador);
                //cout << "Soy el token" << token << " y soy un acronimo" << endl;
            } else if (token[token.size()-1] == '-') {
                TratarMultipalabra(str,delimitadoresPalabra, posDelimitador);
                //cout << "Soy el token" << token << " y soy una multipalabea" << endl;
            }
            token = str.substr(primerCaracter, posDelimitador - primerCaracter);
            tokens.push_back(token);
            if (primerToken) {
                primerToken = false;
            }
        }
        primerCaracter = str.find_first_not_of(delimitadoresPalabra, posDelimitador);
        posDelimitador = str.find_first_of(delimitadoresPalabra, primerCaracter);
    }
}

void
Tokenizador::TratarURL(const string &str,string& delimitadoresPalabra, size_t& posDelimitador){
    size_t i = posDelimitador;
    string delim = "_:/.?&-=#@";
    auto delimAux = delim.c_str();
    do {
        i = str.find_first_of(delimitadoresPalabra, ++i);
        if (i == string::npos) {
            if (posDelimitador + 1 != str.size()) {
                posDelimitador = str.size();
            }
            break;
        } else if (i != str.find_first_of(delimAux, i)) {
            posDelimitador = i;
            break;
        }
    } while (1);
}

void
Tokenizador::TratarEmail(const string &str,string& delimitadoresPalabra, size_t& posDelimitador){
    size_t i = posDelimitador;
    string delim = "_:/.?&-=#";
    auto delimAux = delim.c_str();
    do {
        i = str.find_first_of(delimitadoresPalabra, ++i);
        if (i == string::npos) {
            if (posDelimitador + 1 != str.size()) {
                posDelimitador = str.size();
            }
            break;
        } else if (i == str.find('@', i)) { // mas de 1 @
            break;
        } else if (i != str.find_first_of(delimAux, i)) {
            posDelimitador = i;
            break;
        }
    } while (1);
}

void
Tokenizador::TratarAcronimo (const string &str,string& delimitadoresPalabra,size_t& posDelimitador){
    size_t i = posDelimitador;
    do {
        i = str.find_first_of(delimitadoresPalabra, ++i);
        if (i == string::npos) {
            if (posDelimitador + 1 != str.size()) {
                posDelimitador = str.size();
            }
            break;
        } else if (i != str.find('.', i)) {
            if (i != posDelimitador + 1) {
                posDelimitador = i;
            }
            break;
        } else {
            if (i == posDelimitador + 1) { // no se guarda el ultimo .
                break;
            }
            posDelimitador = i;
        }
    } while (1);
}

void
Tokenizador::TratarMultipalabra(const string &str,string& delimitadoresPalabra, size_t& posDelimitador){
    size_t i = posDelimitador;
    do {
        i = str.find_first_of(delimitadoresPalabra, ++i);
        if (i == string::npos) {
            if (posDelimitador + 1 != str.size()) {
                posDelimitador = str.size();
            }
            break;
        } else if (i != str.find('-', i)) {
            if (i != posDelimitador + 1) {
                posDelimitador = i;
            }
            break;
        } else {
            if (i == posDelimitador + 1) { // no se guarda el ultimo -
                break;
            }
            posDelimitador = i;
        }
    } while (1);
}


bool
Tokenizador::IsDecimal(const string &token) const{

    if (isDecimal && (token[token.size()-1] == '.' || token[token.size()-1] == ',' || token[token.size()-1] == ' '
    || token[0] == '.' || token[0] == ',' || token[0] == ' ')) {
        size_t caracter = token.find_first_of("$%", token.size() - 2);

        if (caracter != string::npos && token.size() == 3) {
            return false;
        }

        for (int i = 1; i < token.size() - 1; ++i) {
            //no son numeros
            if (!(token[i] >= '0' && token[i] <= '9') && token[i] != ',' &&
                token[i] != '.' && caracter == string::npos) {
                return false;
            }
        }

        return true;
    } else return false;


    /*
    if (isDecimal && (token[0] == ' ' || token[0] == ',' || token[0] == '.' ||
        token.back() == ' ' || token.back() == ',' || token.back() == '.')) {

        size_t symbol = token.find_first_of("$%", token.size() - 2);

        for (size_t i = 1; i < token.size() - 1; ++i) {
            if ((token[i] < '0' || token[i] > '9') && token[i] != ',' &&
                token[i] != '.' && symbol == string::npos) {
                return false;
            }
        }

        if (symbol != string::npos && token.size() == 3) {
            return false;
        }

        return true;
    }
    return false;
     */
}

void
Tokenizador::PasarAminuscSinAcentosFun(string& str)const{
    unsigned char caracter;
    for (int i = 0; i < str.size(); i++) {
        caracter = str[i];
        //a-z
        if(caracter >= 97 && caracter <= 122){
            continue;
        }
        //A hasta Z
        else if(caracter >= 65 && caracter <= 90){
            //poner a minuscula
            str[i] = caracter + 32;
        }else{
            //a
            if ((192 <= caracter && caracter <= 197) || (224 <= caracter && caracter  <= 229)) {
                str[i] = 97;
            }
            //e
            else if((200 <= caracter && caracter <= 203) || (232 <= caracter && caracter <= 235)){
                str[i] = 101;
            }
            //i
            else if(204 <= caracter && caracter <= 207 || 236 <= caracter && caracter <= 239){
                str[i] = 105;
            }
            //o
            else if(210 <= caracter && caracter <= 214 || 242 <= caracter && caracter <= 246){
                str[i] = 111;
            }
            //u
            else if(217 <= caracter && caracter <= 220 || 249 <= caracter && caracter <= 252){
                str[i] = 117;
            }
            //y
            else if(caracter == 255 || caracter == 253 || caracter == 221){
                str[i] = 121;
            }
            //ñ
            else if(caracter == 209){
                str[i] = 241;
            }
            //ç
            else if(caracter == 231 || caracter == 199){
                str[i] = 99;
            }
        }
    }
}

bool
Tokenizador::Tokenizar (const string& NomFichEntr, const string& NomFichSal) {
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
            //leer todo el fichero?
            cadena="";
            getline(i, cadena);
            //Verifica que no haya de longitud 0
            char aux[cadena.length()];
            for ( int i = 0; i <cadena.length();i++){
                aux[i]=cadena[i];
            }
            if(cadena.length()!=0)
            {
                Tokenizar(aux, tokens);
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
Tokenizador::Tokenizar (const string & i) {
    return Tokenizar(i,i+".tk");
}

bool
Tokenizador::TokenizarListaFicheros (const string& i) {

}

bool
Tokenizador::TokenizarDirectorio (const string& dirAIndexar)  {
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
Tokenizador::DelimitadoresPalabra (const string& nuevoDelimiters){
    delimiters = PreparacionDelimitadores(nuevoDelimiters);
}

void
Tokenizador::AnyadirDelimitadoresPalabra (const string& nuevoDelimiters){
    delimiters = PreparacionDelimitadores(delimiters+nuevoDelimiters);
}

string
Tokenizador::DelimitadoresPalabra () const{
    return delimiters;
}

void
Tokenizador::CasosEspeciales (const bool& nuevoCasosEspeciales){
    casosEspeciales = nuevoCasosEspeciales;
}

bool
Tokenizador::CasosEspeciales () const{
    return casosEspeciales;
}

void
Tokenizador::PasarAminuscSinAcentos (const bool& nuevoPasarAminuscSinAcentos){
    pasarAminuscSinAcentos = nuevoPasarAminuscSinAcentos;
}

bool
Tokenizador::PasarAminuscSinAcentos () const{
    return pasarAminuscSinAcentos;
}

//Funciones auxiliares
string
Tokenizador::PreparacionDelimitadores(const string& delimitadores){

    int length=delimitadores.length();

    char token[length];

    int num = 0;
    bool repetido = false;

    if(length != 0) {
        token[num] = delimitadores[0];
        num ++;
        if(length > 1) {
            for (int i = 1; i < length; i++) {
                for (int l = 0; l < i; l++) {
                    if (delimitadores[i] == delimitadores[l]) {
                        repetido = true;
                        break;
                    }
                }

                if(!repetido) {
                    token[num] = delimitadores[i];
                    num ++;
                }
                repetido = false;
            }
        }
    }
    token[num]='\0';
    PosiblesCasosEspeciales(token);
    return token;
}

void
Tokenizador::PosiblesCasosEspeciales (string delimitadores) {
    isDecimal =false;
    isUrl = false;

    bool auxDecimal1=false;
    bool auxDecimal2=false;

    for(int i = 0; i < delimitadores.length(); i++){
        if(!auxDecimal1 && delimitadores[i] == '.'){
            auxDecimal1 = true;
        }
        if(!auxDecimal2 && delimitadores[i] == ','){
            auxDecimal2 = true;
        }

        if(!isDecimal && auxDecimal1 && auxDecimal2){
            isDecimal = true;
            //cout << "Puede ser decimal" <<endl;
        }
        if(!isUrl && URL.find_first_of(delimitadores[i]) != std::string::npos){
            isUrl = true;
            //cout << "Puede ser url" <<endl;
        }
    }
}
