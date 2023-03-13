#include "Tokenizador.h"

using namespace std;

Tokenizador::Tokenizador (const char *delimitadoresPalabra, const bool& kcasosEspeciales, const bool& minuscSinAcentos) {
    delimiters = PreparacionDelimitadores(delimitadoresPalabra);
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
Tokenizador::Tokenizar (string& str, list<string>& tokens) const {
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
Tokenizador::AuxTokenizar (const string& str, list<string>& tokens,string delimitadoresPalabra) const {
    bool primerToken = true;
    string token;
    string::size_type primerCaracter = str.find_first_not_of(delimitadoresPalabra,0);
    string::size_type posDelimitador = str.find_first_of(delimitadoresPalabra,primerCaracter);
    while(posDelimitador != string::npos || primerCaracter != string::npos) {
        if (!casosEspeciales) {
            tokens.push_back(str.substr(primerCaracter, posDelimitador - primerCaracter));
        } else {
            if (primerToken) {
                token = str.substr(primerCaracter, posDelimitador - primerCaracter);
            } else {
                token = str.substr(primerCaracter - 1, posDelimitador - primerCaracter + 2);
            }

            //token = str.substr(primerCaracter == 0 ? 0 : (primerCaracter - 1),posDelimitador - primerCaracter + (primerCaracter == 0 ? 1 : 2));
            cout<<token<<endl;
            cout << str.substr(primerCaracter == 0 ? 0 : (primerCaracter - 1),
                               posDelimitador - primerCaracter + (primerCaracter == 0 ? 1 : 2)) << endl;

            if (isUrl && primerToken && (token.find("http:") == 0 || token.find("https:") == 0 || token.find("ftp:") == 0)
                || (delimitadoresPalabra.find(token[0]) != string::npos && (token.find("http:") == 1
                || token.find("https:") == 1 ||token.find("ftp:") == 1))) {
                TratarURL(str, primerCaracter, posDelimitador, delimitadoresPalabra);
            }
            else if (IsDecimal(token)){
                cout << "Soy el token" << token << " y soy un decimal" << endl;
                if (token[0] == '.' || token[0] == ',') {
                    string aux = token;
                    aux = "0" + aux;
                    tokens.push_back(aux);
                    primerCaracter = str.find_first_not_of(delimitadoresPalabra, posDelimitador);
                    posDelimitador = str.find_first_of(delimitadoresPalabra, primerCaracter);
                    continue;
                }
            }else if (token[token.size()-1] == '@') {
                TratarEmail(str, primerCaracter, posDelimitador, delimitadoresPalabra);
                cout << "Soy el token" << token << " y soy un mail" << endl;
            } else if (token[token.size()-1] == '.') {
                TratarAcronimo(str, primerCaracter, posDelimitador, delimitadoresPalabra);
                cout << "Soy el token" << token << " y soy un acronimo" << endl;
            } else if (token[token.size()-1] == '-') {
                TratarMultipalabra(str, primerCaracter, posDelimitador, delimitadoresPalabra);
                cout << "Soy el token" << token << " y soy una multipalabea" << endl;
            }
            token = str.substr(primerCaracter, posDelimitador - primerCaracter);
            tokens.push_back(token);
            //cout << token[token.size()-1] << endl;
            //cout << token.back() << endl;
            // tokens.push_back(str.substr(primerCaracter, posDelimitador - primerCaracter));
            if (primerToken) {
                primerToken = false;
            }
        }
        primerCaracter = str.find_first_not_of(delimitadoresPalabra, posDelimitador);
        posDelimitador = str.find_first_of(delimitadoresPalabra, primerCaracter);
    }
}

void Tokenizador::TratarURL(const string &str, size_t &firstPos, size_t &lastPos,
                               string &delimitadores) const {
    size_t i = lastPos;
    string delim = "_:/.?&-=#@";
    auto delimAux = delim.c_str();
    do {
        i = str.find_first_of(delimitadores, ++i);
        if (i == string::npos) {
            if (lastPos + 1 != str.size()) {
                lastPos = str.size();
            }
            break;
        } else if (i != str.find_first_of(delimAux, i)) {
            lastPos = i;
            break;
        }
    } while (1);
}

void Tokenizador::TratarEmail(const string &str, size_t &firstPos, size_t &lastPos, string &delimitadores) const {
    size_t i = lastPos;
    string delim = "_:/.?&-=#";
    auto delimAux = delim.c_str();
    do {
        i = str.find_first_of(delimitadores, ++i);
        if (i == string::npos) {
            if (lastPos + 1 != str.size()) {
                lastPos = str.size();
            }
            break;
        } else if (i == str.find('@', i)) { // mas de 1 @
            break;
        } else if (i != str.find_first_of(delimAux, i)) {
            lastPos = i;
            break;
        }
    } while (1);
}

void Tokenizador::TratarAcronimo (const string &str, size_t &firstPos, size_t &lastPos, string &delimitadores) const {
    size_t i = lastPos;
    do {
        i = str.find_first_of(delimitadores, ++i);
        if (i == string::npos) {
            if (lastPos + 1 != str.size()) {
                lastPos = str.size();
            }
            break;
        } else if (i != str.find('.', i)) {
            if (i != lastPos + 1) {
                lastPos = i;
            }
            break;
        } else {
            if (i == lastPos + 1) { // no se guarda el ultimo .
                break;
            }
            lastPos = i;
        }
    } while (1);
}

void Tokenizador::TratarMultipalabra(const string &str, size_t &firstPos, size_t &lastPos,
                                     string &delimitadores) const {
    size_t i = lastPos;
    do {
        i = str.find_first_of(delimitadores, ++i);
        if (i == string::npos) {
            if (lastPos + 1 != str.size()) {
                lastPos = str.size();
            }
            break;
        } else if (i != str.find('-', i)) {
            if (i != lastPos + 1) {
                lastPos = i;
            }
            break;
        } else {
            if (i == lastPos + 1) { // no se guarda el ultimo -
                break;
            }
            lastPos = i;
        }
    } while (1);
}


bool
Tokenizador::IsDecimal(const string &token) const{
    if (isDecimal && (token[token.size()-1] == '.' || token[token.size()-1] == ',' || token[token.size()-1] == ' '
    || token[0] == '.' || token[0] == ',' || token[0] == ' ')) {

        //cualquier caracter al principio de la cadena
        size_t caracter = token.find_first_of("^%", token.size() - 2);

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
            if ((192 <= caracter && caracter <= 197) || (224 <= caracter && caracter  <= 229)) {
                str[i] = 97;
            }
            //e
            else if((200 <= caracter && caracter <= 203) || (224 <= caracter && caracter <= 229)){
                str[i] = 101;
            }
            //i
            else if(204 <= caracter && caracter <= 207 || 236 <= caracter && caracter <= 239){
                str[i] = 105;
            }
            //o
            else if(210 <= caracter && caracter <= 215 || 242 <= caracter && caracter <= 246){
                str[i] = 111;
            }
            //u
            else if(217 <= caracter && caracter <= 220 || 249 <= caracter && caracter <= 252){
                str[i] = 117;
            }
            //y
            else if(caracter == 152 || caracter == 221){
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

void
Tokenizador::DetectarUrl(const string& str, list<string>& tokens,string delimitadoresPalabra) const {

    int length = str.length();

    if(length >= 7){
        if((str.find("ftp:") != std::string::npos && delimiters.find(str[4]) == std::string::npos) ||
        (str.find("http:") != std::string::npos && delimiters.find(str[5]) == std::string::npos) ||
        (str.find("https:") != std::string::npos && delimiters.find(str[6]) == std::string::npos)){

        }
    } else if (length >= 6){
        if((str.find("ftp:") != std::string::npos && delimiters.find(str[4]) == std::string::npos) ||
        (str.find("http:") != std::string::npos && delimiters.find(str[5]) == std::string::npos)){

        }
    } else if (length >= 5){
        if(str.find("ftp:") != std::string::npos && delimiters.find(str[4]) == std::string::npos){

        }
    }
}

/*
bool
Tokenizador::DetectarDecimal(list<string>& tokens) const{

}

bool
Tokenizador::DetectarMail(list<string>& tokens) const{

}

bool
Tokenizador::DetectarAcronimo(list<string>& tokens) const{

}

bool
Tokenizador::DetectarGuion(list<string>& tokens) const{

}
 */



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
            //leer todo el fichero?
            cadena="";
            getline(i, cadena);
            //Verifica que no haya de longitud 0
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
    return Tokenizar(i,i+".tk");
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

    for(int i = 0; i < delimitadores.length(); i++){
        if(!isDecimal && decimal.find(delimitadores[i]) != std::string::npos){
            isDecimal = true;
            cout << "Puede ser decimal" <<endl;
        }
        if(!isUrl && URL.find_first_of(delimitadores[i]) != std::string::npos){
            isUrl = true;
            cout << "Puede ser url" <<endl;
        }
    }
}

bool
Tokenizador::IsNum(string& str) const{
    if(str.find_first_not_of("0123456789") == std::string::npos){
        return true;
    } else return false;
}
/*
list<string>
Tokenizador::DetectarCasosEspeciales(const char& str) const{

    int length = str.length();
    //Verificamos que la
    if ((length >= 5 && str.find("ftp:") != std::string::npos && delimiters.find(str[4]) == std::string::npos) ||
    (length >= 6 && str.find("http:") != std::string::npos && delimiters.find(str[5]) == std::string::npos) ||
    (length >= 7 && str.find("https:") != std::string::npos && delimiters.find(str[6]) == std::string::npos)){

        if(length >= 7){
            if((str.find("ftp:") != std::string::npos && delimiters.find(str[4]) == std::string::npos) ||
            (str.find("http:") != std::string::npos && delimiters.find(str[5]) == std::string::npos) ||
            (str.find("https:") != std::string::npos && delimiters.find(str[6]) == std::string::npos)){
                return "url";
            }
        } else if (length >= 6){
            if((str.find("ftp:") != std::string::npos && delimiters.find(str[4]) == std::string::npos) ||
               (str.find("http:") != std::string::npos && delimiters.find(str[5]) == std::string::npos)){
                return "url";
            }
        } else if (length >= 5){
            if(str.find("ftp:") != std::string::npos && delimiters.find(str[4]) == std::string::npos){
                return "url";
            }
        }

        return "url";
    } else if(str.find_first_not_of("0123456789,.") == std::string::npos) {
        if(str[str.length()-1] != ','|| str[str.length()-1] != '.'){
            return "decimal liada";
        }
        return "decimal";
    } else if(){

    }
}
*/




/*
bool
Tokenizador::DetectarUrl(list<string>& tokens) const{

}

bool
Tokenizador::DetectarDecimal(list<string>& tokens) const{

}

bool
Tokenizador::DetectarMail(list<string>& tokens) const{

}

bool
Tokenizador::DetectarAcronimo(list<string>& tokens) const{

}

bool
Tokenizador::DetectarGuion(list<string>& tokens) const{

}
*/
