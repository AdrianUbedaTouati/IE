#ifndef PRACTICA_2_INDEXADORHASH_H
#define PRACTICA_2_INDEXADORHASH_H

#include <iostream>
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
    * “fichStopWords” será el nombre del archivo que contendrá todas las
    * palabras de parada (una palabra por cada línea del fichero) y se
    * almacenará en el campo privado “ficheroStopWords”. Asimismo, almacenará
    * todas las palabras de parada que contenga el archivo en el campo privado
    * “stopWords”, el índice de palabras de parada.
    *
    * “delimitadores” será el string que contiene todos los delimitadores utilizados
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
    IndexadorHash(const string& fichStopWords, const string& delimitadores,
                  const bool& detectComp, const bool& minuscSinAcentos, const string&
                  dirIndice, const int& tStemmer, const bool& almEnDisco, const bool& almPosTerm);

    /**
     * Constructor para inicializar IndexadorHash a partir de una indexación previamente realizada
     * que habrá sido almacenada en “directorioIndexacion” mediante el método “bool GuardarIndexacion()”.
     * Con ello toda la parte privada se inicializará convenientemente, igual
     * que si se acabase de indexar la colección de documentos. En caso que no
     * exista el directorio o que no contenga los datos de la indexación se
     * tratará la excepción correspondiente
     * */
    IndexadorHash(const string& directorioIndexacion);

    IndexadorHash(const IndexadorHash&);

    ~IndexadorHash();

    IndexadorHash& operator= (const IndexadorHash&);

    /**
     * Devuelve true si consigue crear el índice para la colección de documentos detallada en
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

    /**
     * Devuelve true si consigue crear el índice para la colección de documentos
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

    /**
    * Se guardará en disco duro (directorio contenido en la variable privada “directorioIndice”)
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

    /** Vacía la indexación que tuviese en ese momento e inicializa
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
        for(const pair<string,InformacionTermino>& elemento: indice){
            cout << elemento.first << '\t' << elemento.second << endl;
        }
        cout << "Documentos indexados: " << endl;
        // A continuación aparecerá un listado del contenido del campo privado
        // “indiceDocs” donde para cada documento indexado se imprimirá:
        for(const pair<string,InfDoc>& documento: indiceDocs){
            cout << documento.first << '\t' << documento.second << endl;
        }
    }

    /** Devuelve true si consigue crear el índice para la pregunta “preg”.
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

    /** Devuelve true si hay una pregunta indexada (con al menos un término
    * que no sea palabra de parada, o sea, que haya algún término indexado en
    * indicePregunta), devolviéndo “pregunta” en “preg”
    * */
    bool DevuelvePregunta(string& preg) const;

    /** Devuelve true si word (aplicándole el tratamiento de stemming y
    * mayúsculas correspondiente) está indexado en la pregunta, devolviendo su
    * información almacenada “inf”. En caso que no esté, devolvería “inf” vacío
    * */
    bool DevuelvePregunta(const string& word, InformacionTerminoPregunta& inf) const;

    /** Devuelve true si hay una pregunta indexada, devolviendo su
     * información almacenada (campo privado “infPregunta”) en “inf”. En caso
     * que no esté, devolvería “inf” vacío
     * */
    bool DevuelvePregunta(InformacionPregunta& inf) const;

    void ImprimirIndexacionPregunta() {
        cout << "Pregunta indexada: " << pregunta << endl;
        cout << "Terminos indexados en la pregunta: " << endl;
        /* A continuación aparecerá un listado del contenido de
         * “indicePregunta” donde para cada término indexado se imprimirá:
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

    /** Devuelve true si word (aplicándole el tratamiento de stemming y
     * mayúsculas correspondiente) está indexado, devolviendo su información
     * almacenada “inf”. En caso que no esté, devolvería “inf” vacío
     * */
    bool Devuelve(const string& word, InformacionTermino& inf) const;

    /** Devuelve true si word (aplicándole el tratamiento de stemming y
     * mayúsculas correspondiente) está indexado y aparece en el documento de
     * nombre nomDoc, en cuyo caso devuelve la información almacenada para word
     * en el documento. En caso que no esté, devolvería “InfDoc” vacío
     * */
    bool Devuelve(const string& word, const string& nomDoc, InfTermDoc& InfDoc) const;

    /** Devuelve true si word (aplicándole el tratamiento de stemming y
     * mayúsculas correspondiente) aparece como término indexado
     * */
    bool Existe(const string& word) const;

    /** Devuelve true si se realiza el borrado (p.ej. si word, aplicándole el
     * tratamiento de stemming y mayúsculas correspondiente, aparecía como
     * término indexado)
     * */
    bool Borra(const string& word);

    /** Devuelve true si nomDoc está indexado y se realiza el borrado de
     * todos los términos del documento y del documento en los campos privados
     * “indiceDocs” e “informacionColeccionDocs”
     * */
    bool BorraDoc(const string& nomDoc);

    /** Borra todos los términos del índice de documentos: toda la indexación
     * de documentos.
     * */
    void VaciarIndiceDocs();

    /** Borra todos los términos del índice de la pregunta: toda la
     * indexación de la pregunta actual.
     * */
    void VaciarIndicePreg();

    /** Será true si word (aplicándole el tratamiento de stemming y
     * mayúsculas correspondiente) está indexado, sustituyendo la información almacenada por “inf”
     * */
    bool Actualiza(const string& word, const InformacionTermino& inf);

    /** Será true si se realiza la inserción (p.ej. si word, aplicándole el
     * tratamiento de stemming y mayúsculas correspondiente, no estaba
     * previamente indexado)
     * */
    bool Inserta(const string& word, const InformacionTermino& inf);

    /** Devolverá el número de términos diferentes indexados (cardinalidad de
     * campo privado “índice”)
     * */
    int NumPalIndexadas() const;

    // Devuelve el contenido del campo privado “ficheroStopWords”
    string DevolverFichPalParada () const;

    /** Mostrará por pantalla las palabras de parada almacenadas (originales, sin aplicar stemming):
     * una palabra por línea (salto de línea al final de cada palabra)
     * */
    void ListarPalParada() const;

    // Devolverá el número de palabras de parada almacenadas
    int NumPalParada() const;

    // Devuelve los delimitadores utilizados por el tokenizador
    string DevolverDelimitadores () const;

    // Devuelve si el tokenizador analiza los casos especiales
    bool DevolverCasosEspeciales () const;

    // Devuelve si el tokenizador pasa a minúsculas y sin acentos
    bool DevolverPasarAminuscSinAcentos () const;

    // Devuelve el valor de almacenarPosTerm
    bool DevolverAlmacenarPosTerm () const;

    // Devuelve “directorioIndice” (el directorio del disco duro donde se almacenará el índice)
    string DevolverDirIndice () const;

    /** Devolverá el tipo de stemming realizado en la indexación de acuerdo
     * con el valor indicado en la variable privada “tipoStemmer”
     * */
    int DevolverTipoStemming () const;

    // Devolverá el valor indicado en la variable privada “almEnDisco”
    bool DevolverAlmEnDisco () const;

    // Mostrar por pantalla: cout << informacionColeccionDocs << endl;
    void ListarInfColeccDocs() const;

    /** Mostrar por pantalla el contenido el contenido del campo privado
     * “índice”: cout << termino << ‘\t’ << InformacionTermino << endl;
     * */
    void ListarTerminos() const;

    /** Devuelve true si nomDoc existe en la colección y muestra por pantalla todos los términos
     * indexados del documento con nombre “nomDoc”: cout << termino << ‘\t’ << InformacionTermino
     * << endl; . Si no existe no se muestra nada
     * */
    bool ListarTerminos(const string& nomDoc) const;

    /** Mostrar por pantalla el contenido el contenido del campo privado
     * “indiceDocs”: cout << nomDoc << ‘\t’ << InfDoc << endl;
     * */
    void ListarDocs() const;

    /** Devuelve true si nomDoc existe en la colección y muestra por pantalla
     * el contenido del campo privado “indiceDocs” para el documento con nombre
     * “nomDoc”: cout << nomDoc << ‘\t’ << InfDoc << endl; . Si no existe no se
     * muestra nada
     * */
    bool ListarDocs(const string& nomDoc) const;

private:
    /** Este constructor se pone en la parte privada porque no se permitirá
     * crear un indexador sin inicializarlo convenientemente. La inicialización
     * la decidirá el alumno
     * */
    IndexadorHash();

    // Índice de términos indexados accesible por el término
    unordered_map<string, InformacionTermino> indice;

    // Índice de documentos indexados accesible por el nombre del documento
    unordered_map<string, InfDoc> indiceDocs;

    // Información recogida de la colección de documentos indexada
    InfColeccionDocs informacionColeccionDocs;

    // Pregunta indexada actualmente. Si no hay ninguna indexada, contendría el valor “”
    string pregunta;

    // Índice de términos indexados en una pregunta. Se almacenará en memoria principal
    unordered_map<string, InformacionTerminoPregunta> indicePregunta;

    // Información recogida de la pregunta indexada. Se almacenará en memoria principal
    InformacionPregunta infPregunta;

    /** Palabras de parada. Se almacenará en memoria principal. El filtrado
     * de palabras de parada se realizará, tanto en la pregunta como en los
     * documentos, teniendo en cuenta el parámetro minuscSinAcentos y
     * tipoStemmer. Es decir que se aplicará el mismo proceso a las palabras de
     * parada almacenadas en el fichero antes de realizar el filtrado (p.ej. si
     * se aplica el pasar a minúsculas los términos del documento/pregunta a+
     * indexar, para comprobar si se ha de eliminar el término, éste se
     * comparará con la versión de palabras de parada en minúsculas). Esto se
     * pide así para casos en los que en el documento/pregunta aparezca: “La
     * casa de Él” y estén almacenadas como stopWords “la, el”, si se activa el
     * parámetro minuscSinAcentos, entonces debería filtrar “La, Él”, si no
     * hubiese estado activo ese parámetro, entonces no se hubiesen filtrado.
     * */
    unordered_set<string> stopWords;

    // Nombre del fichero que contiene las palabras de parada
    string ficheroStopWords;

    /** Tokenizador que se usará en la indexación. Se inicializará con los parámetros
     * del constructor: detectComp y minuscSinAcentos, los cuales
     * determinarán qué término se ha de indexar (p.ej. si se activa
     * minuscSinAcentos, entonces se guardarán los términos en minúsculas y sin acentos)
     * */
    Tokenizador tok;

    /** “directorioIndice” será el directorio del disco duro donde se
     * almacenará el índice. En caso que contenga la cadena vacía se creará en
     * el directorio donde se ejecute el indexador
     * */
    string directorioIndice;

    /** 0 = no se aplica stemmer: se indexa el término tal y como aparece tokenizado
     * Los siguientes valores harán que los términos a indexar se les aplique el
     * stemmer y se almacene solo dicho stem.
     * 1 = stemmer de Porter para español
     * 2 = stemmer de Porter para inglés
     * Para el stemmer de Porter se utilizarán los archivos
     * stemmer.cpp y stemmer.h, concretamente las funciones de nombre “stemmer”
     * */
    int tipoStemmer;

    /** Esta opción (cuando almacenarEnDisco == true) está ideada para poder
     * indexar colecciones de documentos lo suficientemente grandes para que su
     * indexación no quepa en memoria, por lo que si es true, mientras se va
     * generando el índice, se almacenará la mínima parte de los índices de los
     * documentos en memoria principal, p.ej. solo la estructura “indice” para
     * saber las palabras indexadas, guardando únicamente punteros a las
     * posiciones de los archivos almacenados en disco que contendrán el resto
     * de información asociada a cada término (lo mismo para indiceDocs). Se
     * valorará el equilibrio para conseguir una indexación independientemente
     * del tamaño del corpus a indexar, pero reduciendo el número de accesos a
     * disco. Para los datos de la indexación de la pregunta no habría que
     * hacer nada. En caso de que esta variable tenga valor false, se
     * almacenará todo el índice en memoria principal (tal y como se ha
     * descrito anteriormente).
     * */
    bool almacenarEnDisco;

    /** Si es true se almacenará la posición en la que aparecen los términos
     * dentro del documento en la clase InfTermDoc
     * */
    bool almacenarPosTerm;

    //Añadido

    unordered_map<string, int> auxDisco;

    void ObtenerInfo();
};

#endif //PRACTICA_2_INDEXADORHASH_H