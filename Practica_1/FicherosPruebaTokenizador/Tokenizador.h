#ifndef PRACTICA_1_TOKENIZADOR_H
#define PRACTICA_1_TOKENIZADOR_H

//necesario en este fichero
#include <iostream>
#include <list>

//necesario en tokenizador.cpp
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>

using namespace std;

class Tokenizador {

    /**
     * cout << “DELIMITADORES: “ << delimiters << “ TRATA CASOS ESPECIALES:
     * “ << casosEspeciales << “ PASAR A MINUSCULAS Y SIN ACENTOS: “ <<
     * pasarAminuscSinAcentos;
     * Aunque se modifique el almacenamiento de los delimitadores por temas
     * de eficiencia, el campo delimiters se imprimirá con el string leído en
     * el tokenizador (tras las modificaciones y eliminación de los caracteres
     * repetidos correspondientes)
     */
    friend ostream& operator<<(ostream&, const Tokenizador&);


public:

    /**
     * Inicializa delimiters a delimitadoresPalabra filtrando que no se
     * introduzcan delimitadores repetidos (de izquierda a derecha, en cuyo
     * caso se eliminarían los que hayan sido repetidos por la derecha);
     * casosEspeciales a kcasosEspeciales; pasarAminuscSinAcentos a
     * minuscSinAcentos
     */
    Tokenizador (string& delimitadoresPalabra, const bool& kcasosEspeciales, const bool& minuscSinAcentos);

    /**
     * Constructor de copia
     */
    Tokenizador (const Tokenizador&);

    /**
     * Inicializa delimiters=",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t\n\r@";
     * casosEspeciales a true; pasarAminuscSinAcentos a false
     */
    Tokenizador ();

    /**
     * Pone delimiters=""
     */
    ~Tokenizador ();

    /**
     * Operador de asignacion
     */
    Tokenizador& operator= (const Tokenizador&);

    /**
     * Tokeniza str devolviendo el resultado en tokens. La lista tokens se
     * vaciará antes de almacenar el resultado de la tokenización.
     */
    void Tokenizar (string& str, list<string>& tokens) const;

    /**
     * Tokeniza el fichero i guardando la salida en el fichero f (una
     * palabra en cada línea del fichero). Devolverá true si se realiza la
     * tokenización de forma correcta; false en caso contrario enviando a cerr
     * el mensaje correspondiente (p.ej. que no exista el archivo i)
     */
    bool Tokenizar (const string& i, const string& f) const;

    /**
     * Tokeniza el fichero i guardando la salida en un fichero de nombre i
     * añadiéndole extensión .tk (sin eliminar previamente la extensión de i
     * por ejemplo, del archivo pp.txt se generaría el resultado en pp.txt.tk),
     * y que contendrá una palabra en cada línea del fichero. Devolverá true si
     * se realiza la tokenización de forma correcta; false en caso contrario
     * enviando a cerr el mensaje correspondiente (p.ej. que no exista el
     * archivo i)
     */
    bool Tokenizar (const string & i) const;

    /**
     * Tokeniza el fichero i que contiene un nombre de fichero por línea
     * guardando la salida en ficheros (uno por cada línea de i) cuyo nombre
     * será el leído en i añadiéndole extensión .tk, y que contendrá una
     * palabra en cada línea del fichero leído en i. Devolverá true si se
     * realiza la tokenización de forma correcta de todos los archivos que
     * contiene i; devolverá false en caso contrario enviando a cerr el mensaje
     * correspondiente (p.ej. que no exista el archivo i, o que se trate de un
     * directorio, enviando a “cerr” los archivos de i que no existan o que
     * sean directorios; luego no se ha de interrumpir la ejecución si hay
     * algún archivo en i que no exista)
     */
    bool TokenizarListaFicheros (const string& i) const;

    /**
     * Tokeniza todos los archivos que contenga el directorio i, incluyendo
     * los de los subdirectorios, guardando la salida en ficheros cuyo nombre
     * será el de entrada añadiéndole extensión .tk, y que contendrá una
     * palabra en cada línea del fichero. Devolverá true si se realiza la
     * tokenización de forma correcta de todos los archivos; devolverá false en
     * caso contrario enviando a cerr el mensaje correspondiente (p.ej. que no
     * exista el directorio i, o los ficheros que no se hayan podido tokenizar)
     */
    bool TokenizarDirectorio (const string& i) const;

    /**
     * Inicializa delimiters a nuevoDelimiters, filtrando que no se
     * introduzcan delimitadores repetidos (de izquierda a derecha, en cuyo
     * caso se eliminarían los que hayan sido repetidos por la derecha)
     */
    void DelimitadoresPalabra(const string& nuevoDelimiters);

    /**
     * Añade al final de “delimiters” los nuevos delimitadores que aparezcan
     * en “nuevoDelimiters” (no se almacenarán caracteres repetidos)
     */
    void AnyadirDelimitadoresPalabra(const string& nuevoDelimiters);

    /**
     * Devuelve “delimiters”
     */

    string DelimitadoresPalabra() const;

    /**
     * Cambia la variable privada “casosEspeciales”
     */

    void CasosEspeciales (const bool& nuevoCasosEspeciales);

    /**
     * Devuelve el contenido de la variable privada “casosEspeciales”
     */
    bool CasosEspeciales () const;

    /**
     * Cambia la variable privada “pasarAminuscSinAcentos”. Atención al
     * formato de codificación del corpus (comando “file” de Linux). Para la
     * corrección de la práctica se utilizará el formato actual (ISO-8859).
     */
    void PasarAminuscSinAcentos (const bool& nuevoPasarAminuscSinAcentos);

    /**
     * Devuelve el contenido de la variable privada “pasarAminuscSinAcentos”
     */
    bool PasarAminuscSinAcentos () const;



    //void str2lowercaseNoAccents(string &str);

private:
    /**
     * Delimitadores de términos. Aunque se
     * modifique la forma de almacenamiento interna para mejorar la eficiencia, este
     * campo debe permanecer para indicar el orden en que se introdujeron los
     * delimitadores
     */
    string delimiters;

    /**
     * Si true detectará palabras compuestas y casos especiales. Sino,
     * trabajará al igual que el algoritmo propuesto en la sección “Versión del
     * tokenizador vista en clase”
     */
    bool casosEspeciales;

    /**
     * Si true pasará el token a minúsculas y quitará acentos, antes de
     * realizar la tokenización
     */
    bool pasarAminuscSinAcentos;

    string URL =  ":/.?&-=#@";

    string decimal= ".,";

    bool isUrl = false;

    bool isDecimal = false;

    /**
     * Quita las repeticiones de los delimitadores, eliminado los elementos
     * mas a la derecha
     * Ademas de añadir los demilitadores ' ' y '\n' en caso de que sean casos especiales
     */
    string PreparacionDelimitadores(const string& delimitadores);

    void PosiblesCasosEspeciales(string delimitadores);

    void AuxTokenizar (const string& str, list<string>& tokens,string delimitadoresPalabra) const;

    list<string> DetectarCasosEspeciales(const char& str) const;

    void DetectarUrl(const string& str, list<string>& tokens,string delimitadoresPalabra) const;

    void DetectarDecimal(const string& str, list<string>& tokens,string delimitadoresPalabra) const;

    void DetectarMail(const string& str, list<string>& tokens,string delimitadoresPalabra) const;

    void DetectarAcronimo(const string& str, list<string>& tokens,string delimitadoresPalabra) const;

    void DetectarGuion(const string& str, list<string>& tokens,string delimitadoresPalabra) const;

    bool IsNum(string& str) const;

    //void initCharTemplate();
    //unsigned char charTemplate[256];

    void PasarAminuscSinAcentosFun(string& str)const;

    bool IsDecimal(const string &token)const;

    void TratarURL(const string &str, size_t &firstPos, size_t &lastPos,
                   string&) const;

    void TratarEmail(const string &str, size_t &firstPos, size_t &lastPos, string&) const;


    void TratarAcronimo (const string &str, size_t &firstPos, size_t &lastPos, string&) const;


    void TratarMultipalabra(const string &str, size_t &firstPos, size_t &lastPos,string&) const;


};

#endif /*PRACTICA_1_TOKENIZADOR_H*/
