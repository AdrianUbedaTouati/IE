#ifndef PRACTICA_2_INDEXADORHASH_H
#define PRACTICA_2_INDEXADORHASH_H

#include <iostream>
#include <list>

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
    IndexadorHash(const string& fichStopWords, const string& delimitadores,
                  const bool& detectComp, const bool& minuscSinAcentos, const string&
    dirIndice, const int& tStemmer, const bool& almEnDisco, const bool& almPosTerm);
    /* “fichStopWords” será el nombre del archivo que contendrá todas las
    * palabras de parada (una palabra por cada línea del fichero) y se
    * almacenará en el campo privado “ficheroStopWords”. Asimismo, almacenará
    * todas las palabras de parada que contenga el archivo en el campo privado
    * “stopWords”, el índice de palabras de parada.
    * */


    /* “delimitadores” será el string que contiene todos los delimitadores utilizados
    * por el tokenizador (campo privado “tok”)
    * detectComp y minuscSinAcentos serán los parámetros que se pasarán al tokenizador
    * “dirIndice” será el directorio del disco duro donde se almacenará el índice
    * (campo privado “directorioIndice”).
    * Si dirIndice=”” entonces se almacenará en el directorio donde se ejecute el programa
    * “tStemmer” inicializará la variable privada “tipoStemmer”:
    * 0 = no se aplica stemmer: se indexa el término tal y como aparece tokenizado
    * 1 = stemmer de Porter para español
    * 2 = stemmer de Porter para inglés
    * “almEnDisco” inicializará la variable privada “almacenarEnDisco”
    * “almPosTerm” inicializará la variable privada “almacenarPosTerm”
    * Los índices (p.ej. índice, indiceDocs e informacionColeccionDocs) quedarán vacíos
    * */
    IndexadorHash(const string& directorioIndexacion);

    /* Constructor para inicializar IndexadorHash a partir de una indexación previamente realizada
     * que habrá sido almacenada en “directorioIndexacion” mediante el método “bool GuardarIndexacion()”.
     * Con ello toda la parte privada se inicializará convenientemente, igual
     * que si se acabase de indexar la colección de documentos. En caso que no
     * exista el directorio o que no contenga los datos de la indexación se
     * tratará la excepción correspondiente
     * */
     IndexadorHash(const IndexadorHash&);

     ~IndexadorHash();

    IndexadorHash& operator= (const IndexadorHash&);

    /* Devuelve true si consigue crear el índice para la colección de documentos detallada en
     * ficheroDocumentos,
     * el cual contendrá un nombre de documento por línea. Los añadirá a los ya existentes
     * anteriormente en el índice.
     * Devuelve falso si no finaliza la indexación (p.ej. por falta de memoria),
     * mostrando el mensaje de error
     * correspondiente, indicando el documento y término en el que se ha quedado,
     * dejando en memoria lo que se haya indexado hasta ese momento.
     * En el caso que aparezcan documentos repetidos, documentos que no existen
     * o que ya estuviesen previamente
     * indexados (ha de coincidir el nombre del documento y el directorio en que se encuentre),
     * se devolverá true,
     * mostrando el mensaje de excepción correspondiente, y se reindexarán
     * (borrar el documento previamente indexado e indexar el nuevo)
     * en caso que la fecha de modificación
     * del documento sea más reciente que la almacenada previamente
     * (class “InfDoc” campo “fechaModificacion”).
     * Los casos de reindexación mantendrán el mismo idDoc.
     * */
    bool Indexar(const string& ficheroDocumentos);

    /* Devuelve true si consigue crear el índice para la colección de documentos
     * que se encuentra en el directorio
     * (y subdirectorios que contenga) dirAIndexar (independientemente de la extensión de los mismos).
     * Se considerará que todos los documentos del directorio serán ficheros de texto.
     * Los añadirá a los ya existentes
     * anteriormente en el índice. Devuelve falso si no finaliza la indexación
     * (p.ej. por falta de memoria o porque no exista “dirAIndexar”),
     * mostrando el mensaje de error correspondiente,
     * indicando el documento y término en el que se ha quedado,
     * dejando en memoria lo que se haya indexado hasta ese momento.
     * En el caso que aparezcan documentos repetidos o que ya estuviesen previamente indexados
     * (ha de coincidir el nombre del documento y el directorio en que se encuentre),
     * se mostrará el mensaje de
     * excepción correspondiente,
     * y se re-indexarán (borrar el documento previamente indexado e indexar el nuevo)
     * en caso que la fecha de modificación del documento sea más reciente que la
     * almacenada previamente
     * (class “InfDoc” campo “fechaModificacion”). Los casos de reindexación mantendrán
     * el mismo idDoc.
     * */
    bool IndexarDirectorio(const string& dirAIndexar);

    /* Se guardará en disco duro (directorio contenido en la variable privada “directorioIndice”)
    * la indexación actualmente en memoria (incluidos todos los parámetros de la parte privada).
    * La forma de almacenamiento la determinará el alumno. El objetivo es que esta indexación
    * se pueda recuperar posteriormente mediante el constructor
     * “IndexadorHash(const string& directorioIndexacion)”.
    * Por ejemplo, supongamos que se ejecuta esta secuencia de comandos:
    * “IndexadorHash a(“./fichStopWords.txt”, “[ ,.”, “./dirIndexPrueba”, 0, false);
    * a.Indexar(“./fichConDocsAIndexar.txt”); a.GuardarIndexacion();”,
    * entonces mediante el comando: “IndexadorHash b(“./dirIndexPrueba”);” se
    * recuperará la indexación realizada en la secuencia anterior, cargándola en “b”
    * Devuelve falso si no finaliza la operación (p.ej. por falta de
    * memoria, o el nombre del directorio contenido en “directorioIndice” no es correcto),
    * mostrando el mensaje de error correspondiente, vaciando
    * los ficheros generados.
    * En caso que no existiese el directorio directorioIndice, habría que crearlo previamente
    * */
    bool GuardarIndexacion() const;

    /* Vacía la indexación que tuviese en ese momento e inicializa
    * IndexadorHash a partir de una indexación previamente realizada que habrá
    * sido almacenada en “directorioIndexacion” mediante el método “bool
    * GuardarIndexacion()”. Con ello toda la parte privada se inicializará
    * convenientemente, igual que si se acabase de indexar la colección de
    * documentos. En caso que no exista el directorio o que no contenga los
    * datos de la indexación se tratará la excepción correspondiente, y se
    * devolverá false, dejando la indexación vacía.
    * */
    bool RecuperarIndexacion (const string& directorioIndexacion);

    void ImprimirIndexacion() const {
        cout << "Terminos indexados: " << endl;
        // A continuación aparecerá un listado del contenido del campo privado "índice"
        // donde para cada término indexado se imprimirá:
        cout << termino << '\t' << InformacionTermino << endl;
        cout << "Documentos indexados: " << endl;
        // A continuación aparecerá un listado del contenido del campo privado
        // “indiceDocs” donde para cada documento indexado se imprimirá:
        cout << nomDoc << '\t' << InfDoc << endl;
    }

    /* Devuelve true si consigue crear el índice para la pregunta “preg”.
    * Antes de realizar la indexación vaciará los campos privados
    * indicePregunta e infPregunta.
    * Generará la misma información que en la indexación de documentos, pero dejándola
     * toda accesible en memoria principal
    * (mediante las variables privadas “pregunta, indicePregunta, infPregunta”)
    * Devuelve falso si no finaliza la operación (p.ej. por falta de
    * memoria o bien si la pregunta no contiene ningún término con contenido),
    * mostrando el mensaje de error correspondiente
    * */
    bool IndexarPregunta(const string& preg);

    /* Devuelve true si hay una pregunta indexada (con al menos un término
    * que no sea palabra de parada, o sea, que haya algún término indexado en
    * indicePregunta), devolviéndo “pregunta” en “preg”
    * */
    bool DevuelvePregunta(string& preg) const;

    /* Devuelve true si word (aplicándole el tratamiento de stemming y
    * mayúsculas correspondiente) está indexado en la pregunta, devolviendo su
    * información almacenada “inf”. En caso que no esté, devolvería “inf” vacío
    * */
    bool DevuelvePregunta(const string& word, InformacionTerminoPregunta& inf) const;

    bool DevuelvePregunta(InformacionPregunta& inf) const;
// Devuelve true si hay una pregunta indexada, devolviendo su
    información almacenada (campo privado “infPregunta”) en “inf”. En caso
    que no esté, devolvería “inf” vacío
    void ImprimirIndexacionPregunta() {
        cout << “Pregunta indexada: ” << pregunta << endl;
        cout << “Terminos indexados en la pregunta: ” << endl;
// A continuación aparecerá un listado del contenido de
        “indicePregunta” donde para cada término indexado se imprimirá:
        cout << termino << ‘\t’ << InformacionTerminoPregunta << endl;
        cout << “Informacion de la pregunta: ” << infPregunta << endl;
    }
    void ImprimirPregunta() {
        cout << “Pregunta indexada: ” << pregunta << endl;
        cout << “Informacion de la pregunta: ” << infPregunta << endl;
    }
    bool Devuelve(const string& word, InformacionTermino& inf) const;
// Devuelve true si word (aplicándole el tratamiento de stemming y
    mayúsculas correspondiente) está indexado, devolviendo su información
    almacenada “inf”. En caso que no esté, devolvería “inf” vacío
    bool Devuelve(const string& word, const string& nomDoc, InfTermDoc& InfDoc)
    const;
// Devuelve true si word (aplicándole el tratamiento de stemming y
    mayúsculas correspondiente) está indexado y aparece en el documento de
    nombre nomDoc, en cuyo caso devuelve la información almacenada para word
    en el documento. En caso que no esté, devolvería “InfDoc” vacío
    bool Existe(const string& word) const;
// Devuelve true si word (aplicándole el tratamiento de stemming y
    mayúsculas correspondiente) aparece como término indexado
    bool Borra(const string& word);
// Devuelve true si se realiza el borrado (p.ej. si word, aplicándole el
    tratamiento de stemming y mayúsculas correspondiente, aparecía como
            término indexado)
    bool BorraDoc(const string& nomDoc);
// Devuelve true si nomDoc está indexado y se realiza el borrado de
    todos los términos del documento y del documento en los campos privados
    “indiceDocs” e “informacionColeccionDocs”
    void VaciarIndiceDocs();
// Borra todos los términos del índice de documentos: toda la indexación
    de documentos.
    void VaciarIndicePreg();
// Borra todos los términos del índice de la pregunta: toda la
    indexación de la pregunta actual.
    bool Actualiza(const string& word, const InformacionTermino& inf);
    // Será true si word (aplicándole el tratamiento de stemming y
    mayúsculas correspondiente) está indexado, sustituyendo la información
    almacenada por “inf”



};

#endif //PRACTICA_2_INDEXADORHASH_H
