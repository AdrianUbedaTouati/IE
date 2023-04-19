#ifndef PRACTICA_2_INDEXADORHASH_H
#define PRACTICA_2_INDEXADORHASH_H


#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_set>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <sys/stat.h>
#include <limits>
#include <list>
#include <unordered_set>
#include "indexadorinformacion.h"
#include "tokenizador.h"
#include "indexadorinformacion.h"

class IndexadorHash {

    friend ostream& operator<<(ostream& s, const IndexadorHash& p) {
        s << "Fichero con el listado de palabras de parada: " << p.ficheroStopWords << endl;
        s << "Tokenizador: " << p.tok << endl;
        s << "Directorio donde se almacenara el indice generado: " << p.directorioIndice << endl;
        s << "Stemmer utilizado: " << p.tipoStemmer << endl;
        s << "Informacion de la coleccion indexada: " <<p.informacionColeccionDocs << endl;
        s << "Se almacenara parte del indice en disco duro: " <<p.almacenarEnDisco << endl;
        s << "Se almacenaran las posiciones de los terminos: " <<p.almacenarPosTerm;
        return s;
    }

public:
    /**
    * âfichStopWordsâ serÃ¡ el nombre del archivo que contendrÃ¡ todas las
    * palabras de parada (una palabra por cada lÃ­nea del fichero) y se
    * almacenarÃ¡ en el campo privado âficheroStopWordsâ. Asimismo, almacenarÃ¡
    * todas las palabras de parada que contenga el archivo en el campo privado
    * âstopWordsâ, el Ã­ndice de palabras de parada.
    *
    * âdelimitadoresâ serÃ¡ el string que contiene todos los delimitadores utilizados
    * por el tokenizador (campo privado âtokâ)
    * detectComp y minuscSinAcentos serÃ¡n los parÃ¡metros que se pasarÃ¡n al tokenizador
    * âdirIndiceâ serÃ¡ el directorio del disco duro donde se almacenarÃ¡ el Ã­ndice
    * (campo privado âdirectorioIndiceâ).
    * Si dirIndice=ââ entonces se almacenarÃ¡ en el directorio donde se ejecute el programa
    * âtStemmerâ inicializarÃ¡ la variable privada âtipoStemmerâ:
    * 0 = no se aplica stemmer: se indexa el tÃ©rmino tal y como aparece tokenizado
    * 1 = stemmer de Porter para espaÃ±ol
    * 2 = stemmer de Porter para inglÃ©s
    * âalmEnDiscoâ inicializarÃ¡ la variable privada âalmacenarEnDiscoâ
    * âalmPosTermâ inicializarÃ¡ la variable privada âalmacenarPosTermâ
    * Los Ã­ndices (p.ej. Ã­ndice, indiceDocs e informacionColeccionDocs) quedarÃ¡n vacÃ­os
    * */
    IndexadorHash(const string& fichStopWords, const string& delimitadores,
                  const bool& detectComp, const bool& minuscSinAcentos, const string&
                  dirIndice, const int& tStemmer, const bool& almEnDisco, const bool& almPosTerm);

    /**
     * Constructor para inicializar IndexadorHash a partir de una indexaciÃ³n previamente realizada
     * que habrÃ¡ sido almacenada en âdirectorioIndexacionâ mediante el mÃ©todo âbool
     * GuardarIndexacion()â.
     * Con ello toda la parte privada se inicializarÃ¡ convenientemente, igual
     * que si se acabase de indexar la colecciÃ³n de documentos. En caso que no
     * exista el directorio o que no contenga los datos de la indexaciÃ³n se
     * tratarÃ¡ la excepciÃ³n correspondiente
     * */
    IndexadorHash(const string& directorioIndexacion);

    IndexadorHash(const IndexadorHash&);

    ~IndexadorHash();

    IndexadorHash& operator= (const IndexadorHash&);

    /**
     * Devuelve true si consigue crear el Ã­ndice para la colecciÃ³n de documentos detallada en
     * docCarpeta,
     * el cual contendrÃ¡ un nombre de documento por lÃ­nea. Los aÃ±adirÃ¡ a los ya existentes
     * anteriormente en el Ã­ndice.
     * Devuelve falso si no finaliza la indexaciÃ³n (p.ej. por falta de memoria),
     * mostrando el mensaje de error
     * correspondiente, indicando el documento y tÃ©rmino en el que se ha quedado,
     * dejando en memoria lo que se haya indexado hasta ese momento.
     * En el caso que aparezcan documentos repetidos, documentos que no existen
     * o que ya estuviesen previamente
     * indexados (ha de coincidir el nombre del documento y el directorio en que se encuentre),
     * se devolverÃ¡ true,
     * mostrando el mensaje de excepciÃ³n correspondiente, y se reindexarÃ¡n
     * (borrar el documento previamente indexado e indexar el nuevo)
     * en caso que la fecha de modificaciÃ³n
     * del documento sea mÃ¡s reciente que la almacenada previamente
     * (class âInfDocâ campo âfechaModificacionâ).
     * Los casos de reindexaciÃ³n mantendrÃ¡n el mismo idDoc.
     * */
    bool Indexar(const string& docCarpeta);

    /**
     * Devuelve true si consigue crear el Ã­ndice para la colecciÃ³n de documentos
     * que se encuentra en el directorio
     * (y subdirectorios que contenga) dirAIndexar (independientemente de la extensiÃ³n de los mismos).
     * Se considerarÃ¡ que todos los documentos del directorio serÃ¡n ficheros de texto.
     * Los aÃ±adirÃ¡ a los ya existentes
     * anteriormente en el Ã­ndice. Devuelve falso si no finaliza la indexaciÃ³n
     * (p.ej. por falta de memoria o porque no exista âdirAIndexarâ),
     * mostrando el mensaje de error correspondiente,
     * indicando el documento y tÃ©rmino en el que se ha quedado,
     * dejando en memoria lo que se haya indexado hasta ese momento.
     * En el caso que aparezcan documentos repetidos o que ya estuviesen previamente indexados
     * (ha de coincidir el nombre del documento y el directorio en que se encuentre),
     * se mostrarÃ¡ el mensaje de
     * excepciÃ³n correspondiente,
     * y se re-indexarÃ¡n (borrar el documento previamente indexado e indexar el nuevo)
     * en caso que la fecha de modificaciÃ³n del documento sea mÃ¡s reciente que la
     * almacenada previamente
     * (class âInfDocâ campo âfechaModificacionâ). Los casos de reindexaciÃ³n mantendrÃ¡n
     * el mismo idDoc.
     * */
    bool IndexarDirectorio(const string& dirAIndexar);

    /**
    * Se guardarÃ¡ en disco duro (directorio contenido en la variable privada âdirectorioIndiceâ)
    * la indexaciÃ³n actualmente en memoria (incluidos todos los parÃ¡metros de la parte privada).
    * La forma de almacenamiento la determinarÃ¡ el alumno. El objetivo es que esta indexaciÃ³n
    * se pueda recuperar posteriormente mediante el constructor
    * âIndexadorHash(const string& directorioIndexacion)â.
    * Por ejemplo, supongamos que se ejecuta esta secuencia de comandos:
    * âIndexadorHash a(â./fichStopWords.txtâ, â[ ,.â, â./dirIndexPruebaâ, 0, false);
    * a.Indexar(â./fichConDocsAIndexar.txtâ); a.GuardarIndexacion();â,
    * entonces mediante el comando: âIndexadorHash b(â./dirIndexPruebaâ);â se
    * recuperarÃ¡ la indexaciÃ³n realizada en la secuencia anterior, cargÃ¡ndola en âbâ
    * Devuelve falso si no finaliza la operaciÃ³n (p.ej. por falta de
    * memoria, o el nombre del directorio contenido en âdirectorioIndiceâ no es correcto),
    * mostrando el mensaje de error correspondiente, vaciando
    * los ficheros generados.
    * En caso que no existiese el directorio directorioIndice, habrÃ­a que crearlo previamente
    * */
    bool GuardarIndexacion() const;

    /** VacÃ­a la indexaciÃ³n que tuviese en ese momento e inicializa
    * IndexadorHash a partir de una indexaciÃ³n previamente realizada que habrÃ¡
    * sido almacenada en âdirectorioIndexacionâ mediante el mÃ©todo
     * âbool
    * GuardarIndexacion()â. Con ello toda la parte privada se inicializarÃ¡
    * convenientemente, igual que si se acabase de indexar la colecciÃ³n de
    * documentos. En caso que no exista el directorio o que no contenga los
    * datos de la indexaciÃ³n se tratarÃ¡ la excepciÃ³n correspondiente, y se
    * devolverÃ¡ false, dejando la indexaciÃ³n vacÃ­a.
    * */
    bool RecuperarIndexacion (const string& directorioIndexacion);

    void ImprimirIndexacion() const {
        cout << "Terminos indexados: " << endl;
        // A continuaciÃ³n aparecerÃ¡ un listado del contenido del campo privado "Ã­ndice"
        // donde para cada tÃ©rmino indexado se imprimirÃ¡:
        for(const pair<string,InformacionTermino>& elemento: indice){
            cout << elemento.first << '\t' << elemento.second << endl;
        }
        cout << "Documentos indexados: " << endl;
        // A continuaciÃ³n aparecerÃ¡ un listado del contenido del campo privado
        // âindiceDocsâ donde para cada documento indexado se imprimirÃ¡:
        for(const pair<string,InfDoc>& documento: indiceDocs){
            cout << documento.first << '\t' << documento.second << endl;
        }
    }

    /** Devuelve true si consigue crear el Ã­ndice para la pregunta âpregâ.
    * Antes de realizar la indexaciÃ³n vaciarÃ¡ los campos privados
    * indicePregunta e infPregunta.
    * GenerarÃ¡ la misma informaciÃ³n que en la indexaciÃ³n de documentos, pero dejÃ¡ndola
     * toda accesible en memoria principal
    * (mediante las variables privadas âpregunta, indicePregunta, infPreguntaâ)
    * Devuelve falso si no finaliza la operaciÃ³n (p.ej. por falta de
    * memoria o bien si la pregunta no contiene ningÃºn tÃ©rmino con contenido),
    * mostrando el mensaje de error correspondiente
    * */
    bool IndexarPregunta(const string& preg);

    /** Devuelve true si hay una pregunta indexada (con al menos un tÃ©rmino
    * que no sea palabra de parada, o sea, que haya algÃºn tÃ©rmino indexado en
    * indicePregunta), devolviÃ©ndo âpreguntaâ en âpregâ
    * */
    bool DevuelvePregunta(string& preg) const;

    /** Devuelve true si word (aplicÃ¡ndole el tratamiento de stemming y
    * mayÃºsculas correspondiente) estÃ¡ indexado en la pregunta, devolviendo su
    * informaciÃ³n almacenada âinfâ. En caso que no estÃ©, devolverÃ­a âinfâ vacÃ­o
    * */
    bool DevuelvePregunta(string& word, InformacionTerminoPregunta& inf);

    /** Devuelve true si hay una pregunta indexada, devolviendo su
     * informaciÃ³n almacenada (campo privado âinfPreguntaâ) en âinfâ. En caso
     * que no estÃ©, devolverÃ­a âinfâ vacÃ­o
     * */
    bool DevuelvePregunta(InformacionPregunta& inf) const;

    void ImprimirIndexacionPregunta() {
        cout << "Pregunta indexada: " << pregunta << endl;
        cout << "Terminos indexados en la pregunta: " << endl;
        /* A continuaciÃ³n aparecerÃ¡ un listado del contenido de
         * âindicePreguntaâ donde para cada tÃ©rmino indexado se imprimirÃ¡:
         * */
        for(const pair<string,InformacionTerminoPregunta>& elemento: indicePregunta){
            cout << elemento.first << '\t' << elemento.second << endl;
        }
        cout << "Informacion de la pregunta: " << infPregunta << endl;
    }

    void ImprimirPregunta() {
        cout << "Pregunta indexada: " << pregunta << endl;
        cout << "Informacion de la pregunta: " << infPregunta << endl;
    }

    /** Devuelve true si word (aplicÃ¡ndole el tratamiento de stemming y
     * mayÃºsculas correspondiente) estÃ¡ indexado, devolviendo su informaciÃ³n
     * almacenada âinfâ. En caso que no estÃ©, devolverÃ­a âinfâ vacÃ­o
     * */
    bool Devuelve(string& word, InformacionTermino& inf);

    /** Devuelve true si word (aplicÃ¡ndole el tratamiento de stemming y
     * mayÃºsculas correspondiente) estÃ¡ indexado y aparece en el documento de
     * nombre nomDoc, en cuyo caso devuelve la informaciÃ³n almacenada para word
     * en el documento. En caso que no estÃ©, devolverÃ­a âInfDocâ vacÃ­o
     * */
    bool Devuelve(string& word, const string& nomDoc, InfTermDoc& InfDoc);

    /** Devuelve true si word (aplicÃ¡ndole el tratamiento de stemming y
     * mayÃºsculas correspondiente) aparece como tÃ©rmino indexado
     * */
    bool Existe(string& word);

    /** Devuelve true si se realiza el borrado (p.ej. si word, aplicÃ¡ndole el
     * tratamiento de stemming y mayÃºsculas correspondiente, aparecÃ­a como
     * tÃ©rmino indexado)
     * */
    bool Borra(string& word);

    /** Devuelve true si nomDoc estÃ¡ indexado y se realiza el borrado de
     * todos los tÃ©rminos del documento y del documento en los campos privados
     * âindiceDocsâ e âinformacionColeccionDocsâ
     * */
    bool BorraDoc(string& nomDoc);

    /** Borra todos los tÃ©rminos del Ã­ndice de documentos: toda la indexaciÃ³n
     * de documentos.
     * */
    void RestablecerDocsIndice();

    /** Borra todos los tÃ©rminos del Ã­ndice de la pregunta: toda la
     * indexaciÃ³n de la pregunta actual.
     * */
    void RestablecerPregIndice();

    /** SerÃ¡ true si word (aplicÃ¡ndole el tratamiento de stemming y
     * mayÃºsculas correspondiente) estÃ¡ indexado, sustituyendo la informaciÃ³n almacenada por âinfâ
     * */
    bool Actualiza(const string& word, const InformacionTermino& inf);

    /** SerÃ¡ true si se realiza la inserciÃ³n (p.ej. si word, aplicÃ¡ndole el
     * tratamiento de stemming y mayÃºsculas correspondiente, no estaba
     * previamente indexado)
     * */
    bool Inserta(const string& word, const InformacionTermino& inf);

    /** DevolverÃ¡ el nÃºmero de tÃ©rminos diferentes indexados (cardinalidad de
     * campo privado âÃ­ndiceâ)
     * */
    int NumPalIndexadas() const;

    // Devuelve el contenido del campo privado âficheroStopWordsâ
    string DevolverFichPalParada () const;

    /** MostrarÃ¡ por pantalla las palabras de parada almacenadas (originales, sin aplicar stemming):
     * una palabra por lÃ­nea (salto de lÃ­nea al final de cada palabra)
     * */
    void ListarPalParada() const;

    // DevolverÃ¡ el nÃºmero de palabras de parada almacenadas
    int NumPalParada() const;

    // Devuelve los delimitadores utilizados por el tokenizador
    string DevolverDelimitadores () const;

    // Devuelve si el tokenizador analiza los casos especiales
    bool DevolverCasosEspeciales () const;

    // Devuelve si el tokenizador pasa a minÃºsculas y sin acentos
    bool DevolverPasarAminuscSinAcentos () const;

    // Devuelve el valor de almacenarPosTerm
    bool DevolverAlmacenarPosTerm () const;

    // Devuelve âdirectorioIndiceâ (el directorio del disco duro donde se almacenarÃ¡ el Ã­ndice)
    string DevolverDirIndice () const;

    /** DevolverÃ¡ el tipo de stemming realizado en la indexaciÃ³n de acuerdo
     * con el valor indicado en la variable privada âtipoStemmerâ
     * */
    int DevolverTipoStemming () const;

    // DevolverÃ¡ el valor indicado en la variable privada âalmEnDiscoâ
    bool DevolverAlmEnDisco () const;

    // Mostrar por pantalla: cout << informacionColeccionDocs << endl;
    void ListarInfColeccDocs() const;

    /** Mostrar por pantalla el contenido el contenido del campo privado
     * âÃ­ndiceâ: cout << termino << â\tâ << InformacionTermino << endl;
     * */
    void ListarTerminos() const;

    /** Devuelve true si nomDoc existe en la colecciÃ³n y muestra por pantalla todos los tÃ©rminos
     * indexados del documento con nombre ânomDocâ: cout << termino << â\tâ << InformacionTermino
     * << endl; . Si no existe no se muestra nada
     * */
    bool ListarTerminos(const string& nomDoc) const;

    /** Mostrar por pantalla el contenido el contenido del campo privado
     * âindiceDocsâ: cout << nomDoc << â\tâ << InfDoc << endl;
     * */
    void ListarDocs() const;

    /** Devuelve true si nomDoc existe en la colecciÃ³n y muestra por pantalla
     * el contenido del campo privado âindiceDocsâ para el documento con nombre
     * ânomDocâ: cout << nomDoc << â\tâ << InfDoc << endl; . Si no existe no se
     * muestra nada
     * */
    bool ListarDocs(const string& nomDoc) const;


private:
    /** Este constructor se pone en la parte privada porque no se permitirÃ¡
     * crear un indexador sin inicializarlo convenientemente. La inicializaciÃ³n
     * la decidirÃ¡ el alumno
     * */
    IndexadorHash();

    // Ãndice de tÃ©rminos indexados accesible por el tÃ©rmino
    unordered_map<string, InformacionTermino> indice;

    // Ãndice de documentos indexados accesible por el nombre del documento
    unordered_map<string, InfDoc> indiceDocs;

    // InformaciÃ³n recogida de la colecciÃ³n de documentos indexada
    InfColeccionDocs informacionColeccionDocs;

    // Pregunta indexada actualmente. Si no hay ninguna indexada, contendrÃ­a el valor ââ
    string pregunta;

    // Ãndice de tÃ©rminos indexados en una pregunta. Se almacenarÃ¡ en memoria principal
    unordered_map<string, InformacionTerminoPregunta> indicePregunta;

    // InformaciÃ³n recogida de la pregunta indexada. Se almacenarÃ¡ en memoria principal
    InformacionPregunta infPregunta;

    /** Palabras de parada. Se almacenarÃ¡ en memoria principal. El filtrado
     * de palabras de parada se realizarÃ¡, tanto en la pregunta como en los
     * documentos, teniendo en cuenta el parÃ¡metro minuscSinAcentos y
     * tipoStemmer. Es decir que se aplicarÃ¡ el mismo proceso a las palabras de
     * parada almacenadas en el fichero antes de realizar el filtrado (p.ej. si
     * se aplica el pasar a minÃºsculas los tÃ©rminos del documento/pregunta a+
     * indexar, para comprobar si se ha de eliminar el tÃ©rmino, Ã©ste se
     * compararÃ¡ con la versiÃ³n de palabras de parada en minÃºsculas). Esto se
     * pide asÃ­ para casos en los que en el documento/pregunta aparezca: âLa
     * casa de Ãlâ y estÃ©n almacenadas como stopWords âla, elâ, si se activa el
     * parÃ¡metro minuscSinAcentos, entonces deberÃ­a filtrar âLa, Ãlâ, si no
     * hubiese estado activo ese parÃ¡metro, entonces no se hubiesen filtrado.
     * */
    unordered_set<string> stopWords;

    // Nombre del fichero que contiene las palabras de parada
    string ficheroStopWords;

    /** Tokenizador que se usarÃ¡ en la indexaciÃ³n. Se inicializarÃ¡ con los parÃ¡metros
     * del constructor: detectComp y minuscSinAcentos, los cuales
     * determinarÃ¡n quÃ© tÃ©rmino se ha de indexar (p.ej. si se activa
     * minuscSinAcentos, entonces se guardarÃ¡n los tÃ©rminos en minÃºsculas y sin acentos)
     * */
    Tokenizador tok;

    /** âdirectorioIndiceâ serÃ¡ el directorio del disco duro donde se
     * almacenarÃ¡ el Ã­ndice. En caso que contenga la cadena vacÃ­a se crearÃ¡ en
     * el directorio donde se ejecute el indexador
     * */
    string directorioIndice;

    /** 0 = no se aplica stemmer: se indexa el tÃ©rmino tal y como aparece tokenizado
     * Los siguientes valores harÃ¡n que los tÃ©rminos a indexar se les aplique el
     * stemmer y se almacene solo dicho stem.
     * 1 = stemmer de Porter para espaÃ±ol
     * 2 = stemmer de Porter para inglÃ©s
     * Para el stemmer de Porter se utilizarÃ¡n los archivos
     * stemmer.cpp y stemmer.h, concretamente las funciones de nombre âstemmerâ
     * */
    int tipoStemmer;

    /** Esta opciÃ³n (cuando almacenarEnDisco == true) estÃ¡ ideada para poder
     * indexar colecciones de documentos lo suficientemente grandes para que su
     * indexaciÃ³n no quepa en memoria, por lo que si es true, mientras se va
     * generando el Ã­ndice, se almacenarÃ¡ la mÃ­nima parte de los Ã­ndices de los
     * documentos en memoria principal, p.ej. solo la estructura âindiceâ para
     * saber las palabras indexadas, guardando Ãºnicamente punteros a las
     * posiciones de los archivos almacenados en disco que contendrÃ¡n el resto
     * de informaciÃ³n asociada a cada tÃ©rmino (lo mismo para indiceDocs). Se
     * valorarÃ¡ el equilibrio para conseguir una indexaciÃ³n independientemente
     * del tamaÃ±o del corpus a indexar, pero reduciendo el nÃºmero de accesos a
     * disco. Para los datos de la indexaciÃ³n de la pregunta no habrÃ­a que
     * hacer nada. En caso de que esta variable tenga valor false, se
     * almacenarÃ¡ todo el Ã­ndice en memoria principal (tal y como se ha
     * descrito anteriormente).
     * */
    bool almacenarEnDisco;

    /** Si es true se almacenarÃ¡ la posiciÃ³n en la que aparecen los tÃ©rminos
     * dentro del documento en la clase InfTermDoc
     * */
    bool almacenarPosTerm;

    //AÃ±adido

    unordered_map<string, int> auxDisco;

    int countFileLines (const string&);

    bool hasEnding (const string&, const string&);

    void ObtenerInfo();

    void GuardarPalabrasParada();

    void IndexarDocumento(const string& name, const int& id, const list<string> tokens);

    void AlmacenarEnDisco(const string& token, const InformacionTermino&);

    string getTermDisco(int line) const;

    unordered_map<string, InfDoc> getIndiceDocs() const;

    InfColeccionDocs getInfColDocs() const;

    unordered_map<string, InformacionTerminoPregunta> getIndicePreg() const;

    unordered_map<string, InformacionTermino> getIndice() const;

    string getPregunta() const;

    const char *nombreDirectorio = "indiceDisco.txt";

};

#endif //PRACTICA_2_INDEXADORHASH_H