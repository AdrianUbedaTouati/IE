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
        s << "Fichero con el listado de palabras de parada: " << p.ficheroStopWords << '\n';
        s << "Tokenizador: " << p.tok << '\n';
        s << "Directorio donde se almacenara el indice generado: " << p.directorioIndice << '\n';
        s << "Stemmer utilizado: " << p.tipoStemmer << '\n';
        s << "Informacion de la coleccion indexada: " <<p.informacionColeccionDocs << '\n';
        s << "Se almacenara parte del indice en disco duro: " <<p.almacenarEnDisco << '\n';
        s << "Se almacenaran las posiciones de los terminos: " <<p.almacenarPosTerm;
        return s;
    }

public:
    /**
    * Ã¢ÂÂfichStopWordsÃ¢ÂÂ serÃÂ¡ el nombre del archivo que contendrÃÂ¡ todas las
    * palabras de parada (una palabra por cada lÃÂ­nea del fichero) y se
    * almacenarÃÂ¡ en el campo privado Ã¢ÂÂficheroStopWordsÃ¢ÂÂ. Asimismo, almacenarÃÂ¡
    * todas las palabras de parada que contenga el archivo en el campo privado
    * Ã¢ÂÂstopWordsÃ¢ÂÂ, el ÃÂ­ndice de palabras de parada.
    *
    * Ã¢ÂÂdelimitadoresÃ¢ÂÂ serÃÂ¡ el string que contiene todos los delimitadores utilizados
    * por el tokenizador (campo privado Ã¢ÂÂtokÃ¢ÂÂ)
    * detectComp y minuscSinAcentos serÃÂ¡n los parÃÂ¡metros que se pasarÃÂ¡n al tokenizador
    * Ã¢ÂÂdirIndiceÃ¢ÂÂ serÃÂ¡ el directorio del disco duro donde se almacenarÃÂ¡ el ÃÂ­ndice
    * (campo privado Ã¢ÂÂdirectorioIndiceÃ¢ÂÂ).
    * Si dirIndice=Ã¢ÂÂÃ¢ÂÂ entonces se almacenarÃÂ¡ en el directorio donde se ejecute el programa
    * Ã¢ÂÂtStemmerÃ¢ÂÂ inicializarÃÂ¡ la variable privada Ã¢ÂÂtipoStemmerÃ¢ÂÂ:
    * 0 = no se aplica stemmer: se indexa el tÃÂ©rmino tal y como aparece tokenizado
    * 1 = stemmer de Porter para espaÃÂ±ol
    * 2 = stemmer de Porter para inglÃÂ©s
    * Ã¢ÂÂalmEnDiscoÃ¢ÂÂ inicializarÃÂ¡ la variable privada Ã¢ÂÂalmacenarEnDiscoÃ¢ÂÂ
    * Ã¢ÂÂalmPosTermÃ¢ÂÂ inicializarÃÂ¡ la variable privada Ã¢ÂÂalmacenarPosTermÃ¢ÂÂ
    * Los ÃÂ­ndices (p.ej. ÃÂ­ndice, indiceDocs e informacionColeccionDocs) quedarÃÂ¡n vacÃÂ­os
    * */
    IndexadorHash(const string& fichStopWords, const string& delimitadores,
                  const bool& detectComp, const bool& minuscSinAcentos, const string&
                  dirIndice, const int& tStemmer, const bool& almEnDisco, const bool& almPosTerm);

    /**
     * Constructor para inicializar IndexadorHash a partir de una indexaciÃÂ³n previamente realizada
     * que habrÃÂ¡ sido almacenada en Ã¢ÂÂdirectorioIndexacionÃ¢ÂÂ mediante el mÃÂ©todo Ã¢ÂÂbool
     * GuardarIndexacion()Ã¢ÂÂ.
     * Con ello toda la parte privada se inicializarÃÂ¡ convenientemente, igual
     * que si se acabase de indexar la colecciÃÂ³n de documentos. En caso que no
     * exista el directorio o que no contenga los datos de la indexaciÃÂ³n se
     * tratarÃÂ¡ la excepciÃÂ³n correspondiente
     * */
    IndexadorHash(const string& directorioIndexacion);

    IndexadorHash(const IndexadorHash&);

    ~IndexadorHash();

    IndexadorHash& operator= (const IndexadorHash&);

    /**
     * Devuelve true si consigue crear el ÃÂ­ndice para la colecciÃÂ³n de documentos detallada en
     * docCarpeta,
     * el cual contendrÃÂ¡ un nombre de documento por lÃÂ­nea. Los aÃÂ±adirÃÂ¡ a los ya existentes
     * anteriormente en el ÃÂ­ndice.
     * Devuelve falso si no finaliza la indexaciÃÂ³n (p.ej. por falta de memoria),
     * mostrando el mensaje de error
     * correspondiente, indicando el documento y tÃÂ©rmino en el que se ha quedado,
     * dejando en memoria lo que se haya indexado hasta ese momento.
     * En el caso que aparezcan documentos repetidos, documentos que no existen
     * o que ya estuviesen previamente
     * indexados (ha de coincidir el nombre del documento y el directorio en que se encuentre),
     * se devolverÃÂ¡ true,
     * mostrando el mensaje de excepciÃÂ³n correspondiente, y se reindexarÃÂ¡n
     * (borrar el documento previamente indexado e indexar el nuevo)
     * en caso que la fecha de modificaciÃÂ³n
     * del documento sea mÃÂ¡s reciente que la almacenada previamente
     * (class Ã¢ÂÂInfDocÃ¢ÂÂ campo Ã¢ÂÂfechaModificacionÃ¢ÂÂ).
     * Los casos de reindexaciÃÂ³n mantendrÃÂ¡n el mismo idDoc.
     * */
    bool Indexar(const string& docCarpeta);

    /**
     * Devuelve true si consigue crear el ÃÂ­ndice para la colecciÃÂ³n de documentos
     * que se encuentra en el directorio
     * (y subdirectorios que contenga) dirAIndexar (independientemente de la extensiÃÂ³n de los mismos).
     * Se considerarÃÂ¡ que todos los documentos del directorio serÃÂ¡n ficheros de texto.
     * Los aÃÂ±adirÃÂ¡ a los ya existentes
     * anteriormente en el ÃÂ­ndice. Devuelve falso si no finaliza la indexaciÃÂ³n
     * (p.ej. por falta de memoria o porque no exista Ã¢ÂÂdirAIndexarÃ¢ÂÂ),
     * mostrando el mensaje de error correspondiente,
     * indicando el documento y tÃÂ©rmino en el que se ha quedado,
     * dejando en memoria lo que se haya indexado hasta ese momento.
     * En el caso que aparezcan documentos repetidos o que ya estuviesen previamente indexados
     * (ha de coincidir el nombre del documento y el directorio en que se encuentre),
     * se mostrarÃÂ¡ el mensaje de
     * excepciÃÂ³n correspondiente,
     * y se re-indexarÃÂ¡n (borrar el documento previamente indexado e indexar el nuevo)
     * en caso que la fecha de modificaciÃÂ³n del documento sea mÃÂ¡s reciente que la
     * almacenada previamente
     * (class Ã¢ÂÂInfDocÃ¢ÂÂ campo Ã¢ÂÂfechaModificacionÃ¢ÂÂ). Los casos de reindexaciÃÂ³n mantendrÃÂ¡n
     * el mismo idDoc.
     * */
    bool IndexarDirectorio(const string& dirAIndexar);

    /**
    * Se guardarÃÂ¡ en disco duro (directorio contenido en la variable privada Ã¢ÂÂdirectorioIndiceÃ¢ÂÂ)
    * la indexaciÃÂ³n actualmente en memoria (incluidos todos los parÃÂ¡metros de la parte privada).
    * La forma de almacenamiento la determinarÃÂ¡ el alumno. El objetivo es que esta indexaciÃÂ³n
    * se pueda recuperar posteriormente mediante el constructor
    * Ã¢ÂÂIndexadorHash(const string& directorioIndexacion)Ã¢ÂÂ.
    * Por ejemplo, supongamos que se ejecuta esta secuencia de comandos:
    * Ã¢ÂÂIndexadorHash a(Ã¢ÂÂ./fichStopWords.txtÃ¢ÂÂ, Ã¢ÂÂ[ ,.Ã¢ÂÂ, Ã¢ÂÂ./dirIndexPruebaÃ¢ÂÂ, 0, false);
    * a.Indexar(Ã¢ÂÂ./fichConDocsAIndexar.txtÃ¢ÂÂ); a.GuardarIndexacion();Ã¢ÂÂ,
    * entonces mediante el comando: Ã¢ÂÂIndexadorHash b(Ã¢ÂÂ./dirIndexPruebaÃ¢ÂÂ);Ã¢ÂÂ se
    * recuperarÃÂ¡ la indexaciÃÂ³n realizada en la secuencia anterior, cargÃÂ¡ndola en Ã¢ÂÂbÃ¢ÂÂ
    * Devuelve falso si no finaliza la operaciÃÂ³n (p.ej. por falta de
    * memoria, o el nombre del directorio contenido en Ã¢ÂÂdirectorioIndiceÃ¢ÂÂ no es correcto),
    * mostrando el mensaje de error correspondiente, vaciando
    * los ficheros generados.
    * En caso que no existiese el directorio directorioIndice, habrÃÂ­a que crearlo previamente
    * */
    bool GuardarIndexacion() const;

    /** VacÃÂ­a la indexaciÃÂ³n que tuviese en ese momento e inicializa
    * IndexadorHash a partir de una indexaciÃÂ³n previamente realizada que habrÃÂ¡
    * sido almacenada en Ã¢ÂÂdirectorioIndexacionÃ¢ÂÂ mediante el mÃÂ©todo
     * Ã¢ÂÂbool
    * GuardarIndexacion()Ã¢ÂÂ. Con ello toda la parte privada se inicializarÃÂ¡
    * convenientemente, igual que si se acabase de indexar la colecciÃÂ³n de
    * documentos. En caso que no exista el directorio o que no contenga los
    * datos de la indexaciÃÂ³n se tratarÃÂ¡ la excepciÃÂ³n correspondiente, y se
    * devolverÃÂ¡ false, dejando la indexaciÃÂ³n vacÃÂ­a.
    * */
    bool RecuperarIndexacion (const string& directorioIndexacion);

    void ImprimirIndexacion() const {
        cout << "Terminos indexados: " << '\n';
        // A continuaciÃÂ³n aparecerÃÂ¡ un listado del contenido del campo privado "ÃÂ­ndice"
        // donde para cada tÃÂ©rmino indexado se imprimirÃÂ¡:
        for(const pair<string,InformacionTermino>& elemento: indice){
            cout << elemento.first << '\t' << elemento.second << '\n';
        }
        cout << "Documentos indexados: " << '\n';
        // A continuaciÃÂ³n aparecerÃÂ¡ un listado del contenido del campo privado
        // Ã¢ÂÂindiceDocsÃ¢ÂÂ donde para cada documento indexado se imprimirÃÂ¡:
        for(const pair<string,InfDoc>& documento: indiceDocs){
            cout << documento.first << '\t' << documento.second << '\n';
        }
    }

    /** Devuelve true si consigue crear el ÃÂ­ndice para la pregunta Ã¢ÂÂpregÃ¢ÂÂ.
    * Antes de realizar la indexaciÃÂ³n vaciarÃÂ¡ los campos privados
    * indicePregunta e infPregunta.
    * GenerarÃÂ¡ la misma informaciÃÂ³n que en la indexaciÃÂ³n de documentos, pero dejÃÂ¡ndola
     * toda accesible en memoria principal
    * (mediante las variables privadas Ã¢ÂÂpregunta, indicePregunta, infPreguntaÃ¢ÂÂ)
    * Devuelve falso si no finaliza la operaciÃÂ³n (p.ej. por falta de
    * memoria o bien si la pregunta no contiene ningÃÂºn tÃÂ©rmino con contenido),
    * mostrando el mensaje de error correspondiente
    * */
    bool IndexarPregunta(const string& preg);

    /** Devuelve true si hay una pregunta indexada (con al menos un tÃÂ©rmino
    * que no sea palabra de parada, o sea, que haya algÃÂºn tÃÂ©rmino indexado en
    * indicePregunta), devolviÃÂ©ndo Ã¢ÂÂpreguntaÃ¢ÂÂ en Ã¢ÂÂpregÃ¢ÂÂ
    * */
    bool DevuelvePregunta(string& preg) const;

    /** Devuelve true si word (aplicÃÂ¡ndole el tratamiento de stemming y
    * mayÃÂºsculas correspondiente) estÃÂ¡ indexado en la pregunta, devolviendo su
    * informaciÃÂ³n almacenada Ã¢ÂÂinfÃ¢ÂÂ. En caso que no estÃÂ©, devolverÃÂ­a Ã¢ÂÂinfÃ¢ÂÂ vacÃÂ­o
    * */
    bool DevuelvePregunta(string& word, InformacionTerminoPregunta& inf);

    /** Devuelve true si hay una pregunta indexada, devolviendo su
     * informaciÃÂ³n almacenada (campo privado Ã¢ÂÂinfPreguntaÃ¢ÂÂ) en Ã¢ÂÂinfÃ¢ÂÂ. En caso
     * que no estÃÂ©, devolverÃÂ­a Ã¢ÂÂinfÃ¢ÂÂ vacÃÂ­o
     * */
    bool DevuelvePregunta(InformacionPregunta& inf) const;

    void ImprimirIndexacionPregunta() {
        cout << "Pregunta indexada: " << pregunta << '\n';
        cout << "Terminos indexados en la pregunta: " << '\n';
        /* A continuaciÃÂ³n aparecerÃÂ¡ un listado del contenido de
         * Ã¢ÂÂindicePreguntaÃ¢ÂÂ donde para cada tÃÂ©rmino indexado se imprimirÃÂ¡:
         * */
        for(const pair<string,InformacionTerminoPregunta>& elemento: indicePregunta){
            cout << elemento.first << '\t' << elemento.second << '\n';
        }
        cout << "Informacion de la pregunta: " << infPregunta << '\n';
    }

    void ImprimirPregunta() {
        cout << "Pregunta indexada: " << pregunta << '\n';
        cout << "Informacion de la pregunta: " << infPregunta << '\n';
    }

    /** Devuelve true si word (aplicÃÂ¡ndole el tratamiento de stemming y
     * mayÃÂºsculas correspondiente) estÃÂ¡ indexado, devolviendo su informaciÃÂ³n
     * almacenada Ã¢ÂÂinfÃ¢ÂÂ. En caso que no estÃÂ©, devolverÃÂ­a Ã¢ÂÂinfÃ¢ÂÂ vacÃÂ­o
     * */
    bool Devuelve(string& word, InformacionTermino& inf);

    /** Devuelve true si word (aplicÃÂ¡ndole el tratamiento de stemming y
     * mayÃÂºsculas correspondiente) estÃÂ¡ indexado y aparece en el documento de
     * nombre nomDoc, en cuyo caso devuelve la informaciÃÂ³n almacenada para word
     * en el documento. En caso que no estÃÂ©, devolverÃÂ­a Ã¢ÂÂInfDocÃ¢ÂÂ vacÃÂ­o
     * */
    bool Devuelve(string& word, const string& nomDoc, InfTermDoc& InfDoc);

    /** Devuelve true si word (aplicÃÂ¡ndole el tratamiento de stemming y
     * mayÃÂºsculas correspondiente) aparece como tÃÂ©rmino indexado
     * */
    bool Existe(string& word);

    /** Devuelve true si se realiza el borrado (p.ej. si word, aplicÃÂ¡ndole el
     * tratamiento de stemming y mayÃÂºsculas correspondiente, aparecÃÂ­a como
     * tÃÂ©rmino indexado)
     * */
    bool Borra(string& word);

    /** Devuelve true si nomDoc estÃÂ¡ indexado y se realiza el borrado de
     * todos los tÃÂ©rminos del documento y del documento en los campos privados
     * Ã¢ÂÂindiceDocsÃ¢ÂÂ e Ã¢ÂÂinformacionColeccionDocsÃ¢ÂÂ
     * */
    bool BorraDoc(string& nomDoc);

    /** Borra todos los tÃÂ©rminos del ÃÂ­ndice de documentos: toda la indexaciÃÂ³n
     * de documentos.
     * */
    void RestablecerDocsIndice();

    /** Borra todos los tÃÂ©rminos del ÃÂ­ndice de la pregunta: toda la
     * indexaciÃÂ³n de la pregunta actual.
     * */
    void RestablecerPregIndice();

    /** SerÃÂ¡ true si word (aplicÃÂ¡ndole el tratamiento de stemming y
     * mayÃÂºsculas correspondiente) estÃÂ¡ indexado, sustituyendo la informaciÃÂ³n almacenada por Ã¢ÂÂinfÃ¢ÂÂ
     * */
    bool Actualiza(const string& word, const InformacionTermino& inf);

    /** SerÃÂ¡ true si se realiza la inserciÃÂ³n (p.ej. si word, aplicÃÂ¡ndole el
     * tratamiento de stemming y mayÃÂºsculas correspondiente, no estaba
     * previamente indexado)
     * */
    bool Inserta(const string& word, const InformacionTermino& inf);

    /** DevolverÃÂ¡ el nÃÂºmero de tÃÂ©rminos diferentes indexados (cardinalidad de
     * campo privado Ã¢ÂÂÃÂ­ndiceÃ¢ÂÂ)
     * */
    int NumPalIndexadas() const;

    // Devuelve el contenido del campo privado Ã¢ÂÂficheroStopWordsÃ¢ÂÂ
    string DevolverFichPalParada () const;

    /** MostrarÃÂ¡ por pantalla las palabras de parada almacenadas (originales, sin aplicar stemming):
     * una palabra por lÃÂ­nea (salto de lÃÂ­nea al final de cada palabra)
     * */
    void ListarPalParada() const;

    // DevolverÃÂ¡ el nÃÂºmero de palabras de parada almacenadas
    int NumPalParada() const;

    // Devuelve los delimitadores utilizados por el tokenizador
    string DevolverDelimitadores () const;

    // Devuelve si el tokenizador analiza los casos especiales
    bool DevolverCasosEspeciales () const;

    // Devuelve si el tokenizador pasa a minÃÂºsculas y sin acentos
    bool DevolverPasarAminuscSinAcentos () const;

    // Devuelve el valor de almacenarPosTerm
    bool DevolverAlmacenarPosTerm () const;

    // Devuelve Ã¢ÂÂdirectorioIndiceÃ¢ÂÂ (el directorio del disco duro donde se almacenarÃÂ¡ el ÃÂ­ndice)
    string DevolverDirIndice () const;

    /** DevolverÃÂ¡ el tipo de stemming realizado en la indexaciÃÂ³n de acuerdo
     * con el valor indicado en la variable privada Ã¢ÂÂtipoStemmerÃ¢ÂÂ
     * */
    int DevolverTipoStemming () const;

    // DevolverÃÂ¡ el valor indicado en la variable privada Ã¢ÂÂalmEnDiscoÃ¢ÂÂ
    bool DevolverAlmEnDisco () const;

    // Mostrar por pantalla: cout << informacionColeccionDocs << '\n';
    void ListarInfColeccDocs() const;

    /** Mostrar por pantalla el contenido el contenido del campo privado
     * Ã¢ÂÂÃÂ­ndiceÃ¢ÂÂ: cout << termino << Ã¢ÂÂ\tÃ¢ÂÂ << InformacionTermino << '\n';
     * */
    void ListarTerminos() const;

    /** Devuelve true si nomDoc existe en la colecciÃÂ³n y muestra por pantalla todos los tÃÂ©rminos
     * indexados del documento con nombre Ã¢ÂÂnomDocÃ¢ÂÂ: cout << termino << Ã¢ÂÂ\tÃ¢ÂÂ << InformacionTermino
     * << endl; . Si no existe no se muestra nada
     * */
    bool ListarTerminos(const string& nomDoc) const;

    /** Mostrar por pantalla el contenido el contenido del campo privado
     * Ã¢ÂÂindiceDocsÃ¢ÂÂ: cout << nomDoc << Ã¢ÂÂ\tÃ¢ÂÂ << InfDoc << endl;
     * */
    void ListarDocs() const;

    /** Devuelve true si nomDoc existe en la colecciÃÂ³n y muestra por pantalla
     * el contenido del campo privado Ã¢ÂÂindiceDocsÃ¢ÂÂ para el documento con nombre
     * Ã¢ÂÂnomDocÃ¢ÂÂ: cout << nomDoc << Ã¢ÂÂ\tÃ¢ÂÂ << InfDoc << endl; . Si no existe no se
     * muestra nada
     * */
    bool ListarDocs(const string& nomDoc) const;


private:
    /** Este constructor se pone en la parte privada porque no se permitirÃÂ¡
     * crear un indexador sin inicializarlo convenientemente. La inicializaciÃÂ³n
     * la decidirÃÂ¡ el alumno
     * */
    IndexadorHash();

    // ÃÂndice de tÃÂ©rminos indexados accesible por el tÃÂ©rmino
    unordered_map<string, InformacionTermino> indice;

    // ÃÂndice de documentos indexados accesible por el nombre del documento
    unordered_map<string, InfDoc> indiceDocs;

    // InformaciÃÂ³n recogida de la colecciÃÂ³n de documentos indexada
    InfColeccionDocs informacionColeccionDocs;

    // Pregunta indexada actualmente. Si no hay ninguna indexada, contendrÃÂ­a el valor Ã¢ÂÂÃ¢ÂÂ
    string pregunta;

    // ÃÂndice de tÃÂ©rminos indexados en una pregunta. Se almacenarÃÂ¡ en memoria principal
    unordered_map<string, InformacionTerminoPregunta> indicePregunta;

    // InformaciÃÂ³n recogida de la pregunta indexada. Se almacenarÃÂ¡ en memoria principal
    InformacionPregunta infPregunta;

    /** Palabras de parada. Se almacenarÃÂ¡ en memoria principal. El filtrado
     * de palabras de parada se realizarÃÂ¡, tanto en la pregunta como en los
     * documentos, teniendo en cuenta el parÃÂ¡metro minuscSinAcentos y
     * tipoStemmer. Es decir que se aplicarÃÂ¡ el mismo proceso a las palabras de
     * parada almacenadas en el fichero antes de realizar el filtrado (p.ej. si
     * se aplica el pasar a minÃÂºsculas los tÃÂ©rminos del documento/pregunta a+
     * indexar, para comprobar si se ha de eliminar el tÃÂ©rmino, ÃÂ©ste se
     * compararÃÂ¡ con la versiÃÂ³n de palabras de parada en minÃÂºsculas). Esto se
     * pide asÃÂ­ para casos en los que en el documento/pregunta aparezca: Ã¢ÂÂLa
     * casa de ÃÂlÃ¢ÂÂ y estÃÂ©n almacenadas como stopWords Ã¢ÂÂla, elÃ¢ÂÂ, si se activa el
     * parÃÂ¡metro minuscSinAcentos, entonces deberÃÂ­a filtrar Ã¢ÂÂLa, ÃÂlÃ¢ÂÂ, si no
     * hubiese estado activo ese parÃÂ¡metro, entonces no se hubiesen filtrado.
     * */
    unordered_set<string> stopWords;

    // Nombre del fichero que contiene las palabras de parada
    string ficheroStopWords;

    /** Tokenizador que se usarÃÂ¡ en la indexaciÃÂ³n. Se inicializarÃÂ¡ con los parÃÂ¡metros
     * del constructor: detectComp y minuscSinAcentos, los cuales
     * determinarÃÂ¡n quÃÂ© tÃÂ©rmino se ha de indexar (p.ej. si se activa
     * minuscSinAcentos, entonces se guardarÃÂ¡n los tÃÂ©rminos en minÃÂºsculas y sin acentos)
     * */
    Tokenizador tok;

    /** Ã¢ÂÂdirectorioIndiceÃ¢ÂÂ serÃÂ¡ el directorio del disco duro donde se
     * almacenarÃÂ¡ el ÃÂ­ndice. En caso que contenga la cadena vacÃÂ­a se crearÃÂ¡ en
     * el directorio donde se ejecute el indexador
     * */
    string directorioIndice;

    /** 0 = no se aplica stemmer: se indexa el tÃÂ©rmino tal y como aparece tokenizado
     * Los siguientes valores harÃÂ¡n que los tÃÂ©rminos a indexar se les aplique el
     * stemmer y se almacene solo dicho stem.
     * 1 = stemmer de Porter para espaÃÂ±ol
     * 2 = stemmer de Porter para inglÃÂ©s
     * Para el stemmer de Porter se utilizarÃÂ¡n los archivos
     * stemmer.cpp y stemmer.h, concretamente las funciones de nombre Ã¢ÂÂstemmerÃ¢ÂÂ
     * */
    int tipoStemmer;

    /** Esta opciÃÂ³n (cuando almacenarEnDisco == true) estÃÂ¡ ideada para poder
     * indexar colecciones de documentos lo suficientemente grandes para que su
     * indexaciÃÂ³n no quepa en memoria, por lo que si es true, mientras se va
     * generando el ÃÂ­ndice, se almacenarÃÂ¡ la mÃÂ­nima parte de los ÃÂ­ndices de los
     * documentos en memoria principal, p.ej. solo la estructura Ã¢ÂÂindiceÃ¢ÂÂ para
     * saber las palabras indexadas, guardando ÃÂºnicamente punteros a las
     * posiciones de los archivos almacenados en disco que contendrÃÂ¡n el resto
     * de informaciÃÂ³n asociada a cada tÃÂ©rmino (lo mismo para indiceDocs). Se
     * valorarÃÂ¡ el equilibrio para conseguir una indexaciÃÂ³n independientemente
     * del tamaÃÂ±o del corpus a indexar, pero reduciendo el nÃÂºmero de accesos a
     * disco. Para los datos de la indexaciÃÂ³n de la pregunta no habrÃÂ­a que
     * hacer nada. En caso de que esta variable tenga valor false, se
     * almacenarÃÂ¡ todo el ÃÂ­ndice en memoria principal (tal y como se ha
     * descrito anteriormente).
     * */
    bool almacenarEnDisco;

    /** Si es true se almacenarÃÂ¡ la posiciÃÂ³n en la que aparecen los tÃÂ©rminos
     * dentro del documento en la clase InfTermDoc
     * */
    bool almacenarPosTerm;

    //AÃÂ±adido

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