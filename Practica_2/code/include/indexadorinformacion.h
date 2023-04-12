#ifndef PRACTICA_2_INDEXADORINFORMACION_H
#define PRACTICA_2_INDEXADORINFORMACION_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <list>
#include <unordered_map>
#include <ctime>

using namespace std;

class InfTermDoc {
    friend ostream &operator<<(ostream &s, const InfTermDoc &p);

public:
    InfTermDoc(const InfTermDoc &);

    // Inicializa ft = 0
    InfTermDoc();

    // Pone ft = 0
    ~InfTermDoc();

    InfTermDoc &operator=(const InfTermDoc &);

    //Auxiliares
    void AumentoFt(){ft++;}

    void nuevoPosTerm(const int& pos){posTerm.push_back(pos);posTerm.sort(less<int>());}

    //Seters/Geters
    void setPosTerm(list<int> aux){aux.sort(less<int>());posTerm = aux;}
    list<int> getPosTerm(){return posTerm;}

    void setFt(int aux){ft = aux;}
    int getFt(){return ft;}

private:
    // Añadir cuantos métodos se consideren necesarios para manejar la parte privada de la clase

    // Frecuencia del término en el documento
    int ft;

    /** Solo se almacenará esta información si el campo privado del indexador almacenarPosTerm == true
     * Lista de números de palabra en los que aparece el término en el
     * documento. Los números de palabra comenzarán desde cero (la primera
     * palabra del documento). Se numerarán las palabras de parada. Estará
     * ordenada de menor a mayor posición.
     * */
    list<int> posTerm;
};

class InformacionTermino {
        friend ostream& operator<<(ostream& s, const InformacionTermino& p);

    public:
        InformacionTermino (const InformacionTermino &);
        // Inicializa ftc = 0
        InformacionTermino ();
        // Pone ftc = 0 y vacía l_docs
        ~InformacionTermino ();

        // Añadir cuantos métodos se consideren necesarios para manejar la parte privada de la clase
        InformacionTermino & operator= (const InformacionTermino &);

        //Auxiliares
        void aumentoFtc(){ftc++;}

        void nuevoL_docs(int &id, InfTermDoc &doc) {
            l_docs.insert(pair<int, InfTermDoc>(id, doc));
        }

        //Seters/Geters
        void setL_docs(unordered_map<int, InfTermDoc> aux){l_docs = aux;}
        unordered_map<int, InfTermDoc> getL_docs(){return l_docs;}

        void setFtc(int aux){ftc = aux;}
        int getFtc(){return ftc;}

    private:
        // Frecuencia total del término en la colección
        int ftc;

        /** Tabla Hash que se accederá por el id del documento, devolviendo un
         * objeto de la clase InfTermDoc que contiene toda la información de
         * aparición del término en el documento
         * */
        unordered_map<int, InfTermDoc> l_docs;
};



class InfDoc {
    friend ostream& operator<<(ostream& s, const InfDoc& p);

public:
    InfDoc (const InfDoc &);
    InfDoc (time_t,int, int, int, int, int);
    ~InfDoc ();
    InfDoc & operator= (const InfDoc &);

    //Auxiliares
    void setFechaModificacion(time_t aux){fechaModificacion = aux;}
    time_t getFechaModificacion(){return fechaModificacion;}

    void setNumPal(int aux){numPal = aux;}
    int getNumPal(){return numPal;}

    void setIdDoc(int aux){idDoc = aux;}
    int getIdDoc(){return idDoc;}

    void setTamBytes(int aux){tamBytes = aux;}
    int getTamBytes(){return tamBytes;}

    void setNumPalSinParada(int aux){numPalSinParada = aux;}
    int getNumPalSinParada(){return numPalSinParada;}

    void setNumPalDiferentes(int aux){numPalDiferentes = aux;}
    int getNumPalDiferentes(){return numPalDiferentes;}

private:
    // Añadir cuantos métodos se consideren necesarios para manejar la parte privada de la clase

    // Identificador del documento. El primer documento indexado en la colección será el identificador 1
    int idDoc;

    // Nº total de palabras del documento
    int numPal;

    // Nº total de palabras sin stop-words del documento
    int numPalSinParada;

    // Nº total de palabras diferentes que no sean stop-words (sin acumular la frecuencia de cada una de ellas)
    int numPalDiferentes;

    // Tamaño en bytes del documento
    int tamBytes;

    /** Atributo correspondiente a la fecha y hora (completa) de modificación del documento.
     * El tipo “Fecha/hora” lo elegirá/implementará el alumno
     * */
    time_t fechaModificacion;
};

class InfColeccionDocs {
    friend ostream& operator<<(ostream& s, const InfColeccionDocs& p);

public:
    InfColeccionDocs (const InfColeccionDocs &);
    InfColeccionDocs (int,int,int,int,int);
    ~InfColeccionDocs ();
    InfColeccionDocs & operator= (const InfColeccionDocs &);

    //Auxiliares
    void setNumDocs(int aux){numDocs = aux;}
    int getNumDocs(){return numDocs;}

    void setNumTotalPal(int aux){numTotalPal = aux;}
    int getNumTotalPal(){return numTotalPal;}

    void setNumTotalPalSinParada(int aux){numTotalPalSinParada = aux;}
    int getNumTotalPalSinParada(){return numTotalPalSinParada;}

    void setNumTotalPalDiferentes(int aux){numTotalPalDiferentes = aux;}
    int getNumTotalPalDiferentes(){return numTotalPalDiferentes;}

    void setTamBytes(int aux){tamBytes = aux;}
    int getTamBytes(){return tamBytes;}

private:
    // Añadir cuantos métodos se consideren necesarios para manejar la parte privada de la clase

    // Nº total de documentos en la colección
    int numDocs;

    // Nº total de palabras en la colección
    int numTotalPal;

    // Nº total de palabras sin stop-words en la colección
    int numTotalPalSinParada;

    /** Nº total de palabras diferentes en la colección que no sean
     * stopwords (sin acumular la frecuencia de cada una de ellas)
     * */
    int numTotalPalDiferentes;

    // Tamaño total en bytes de la colección
    int tamBytes;
};

class InformacionTerminoPregunta {
    friend ostream& operator<<(ostream& s, const InformacionTerminoPregunta& p);

public:
    InformacionTerminoPregunta (const InformacionTerminoPregunta &);
    InformacionTerminoPregunta ();
    ~InformacionTerminoPregunta ();
    InformacionTerminoPregunta & operator= (const InformacionTerminoPregunta &);

    //Auxiliares
    void aumentoFt() {ft++;}

    void nuevoPosTerm(int pos) {posTerm.push_back(pos);}

    //Seters/Geters
    void setFt(int aux){ft = aux;}
    int getFt(){return ft;}

    void setPosTerm(list<int> aux){posTerm = aux;}
    list<int> getPosTerm(){return posTerm;}

private:
    // Añadir cuantos métodos se consideren necesarios para manejar la parte privada de la clase

    // Frecuencia total del término en la pregunta
    int ft;

    /** Solo se almacenará esta información si el campo privado del indexador almacenarPosTerm == true
     * Lista de números de palabra en los que aparece el término en la pregunta.
     * Los números de palabra comenzarán desde cero (la primera
     * palabra de la pregunta). Se numerarán las palabras de parada. Estará
     * ordenada de menor a mayor posición.
     * */
    list<int> posTerm;
};

class InformacionPregunta {
    friend ostream& operator<<(ostream& s, const InformacionPregunta& p);

    public:
        InformacionPregunta (const InformacionPregunta &);
        InformacionPregunta ();
        ~InformacionPregunta ();
        InformacionPregunta & operator= (const InformacionPregunta &);

    //Auxiliares
    void setNumTotalPal(int aux){numTotalPal = aux;}
    int getNumTotalPal(){return numTotalPal;}

    void setNumTotalPalSinParada(int aux){numTotalPalSinParada = aux;}
    int getNumTotalPalSinParada(){return numTotalPalSinParada;}

    void setNumTotalPalDiferentes(int aux){numTotalPalDiferentes = aux;}
    int getNumTotalPalDiferentes(){return numTotalPalDiferentes;}

    private:
        // Añadir cuantos métodos se consideren necesarios para manejar la parte privada de la clase

        // Nº total de palabras en la pregunta
        int numTotalPal;

        // Nº total de palabras sin stop-words en la pregunta
        int numTotalPalSinParada;

        /* Nº total de palabras diferentes en la pregunta que no sean stop-words
         * (sin acumular la frecuencia de cada una de ellas)
         * */
        int numTotalPalDiferentes;
};

#endif //PRACTICA_2_INDEXADORINFORMACION_H