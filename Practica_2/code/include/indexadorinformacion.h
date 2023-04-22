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

    void nuevaPosicionTermino(const int& pos){posTerm.push_back(pos);posTerm.sort(less<int>());}

    //Seters/Geters
    void setPosTerm(list<int> aux){aux.sort(less<int>());posTerm = aux;}
    list<int> getPosTerm()const{return posTerm;}

    void setFt(int aux){ft = aux;}
    int getFt()const{return ft;}

private:
    // AÃÂ±adir cuantos mÃÂ©todos se consideren necesarios para manejar la parte privada de la clase

    // Frecuencia del tÃÂ©rmino en el documento
    int ft;

    /** Solo se almacenarÃÂ¡ esta informaciÃÂ³n si el campo privado del indexador almacenarPosTerm == true
     * Lista de nÃÂºmeros de palabra en los que aparece el tÃÂ©rmino en el
     * documento. Los nÃÂºmeros de palabra comenzarÃÂ¡n desde cero (la primera
     * palabra del documento). Se numerarÃÂ¡n las palabras de parada. EstarÃÂ¡
     * ordenada de menor a mayor posiciÃÂ³n.
     * */
    list<int> posTerm;
};

class InformacionTermino {
        friend ostream& operator<<(ostream& s, const InformacionTermino& p);

    public:
        InformacionTermino (const InformacionTermino &);
        // Inicializa ftc = 0
        InformacionTermino ();
        // Pone ftc = 0 y vacÃÂ­a l_docs
        ~InformacionTermino ();

        // AÃÂ±adir cuantos mÃÂ©todos se consideren necesarios para manejar la parte privada de la clase
        InformacionTermino & operator= (const InformacionTermino &);

        //Auxiliares
        void aumentoFtc(){ftc++;}

        void nuevoL_docs(const int &id, InfTermDoc &doc) {
            l_docs.insert(pair<int, InfTermDoc>(id, doc));
        }

        //Seters/Geters
        void setL_docs(unordered_map<int, InfTermDoc> aux){l_docs = aux;}
        unordered_map<int, InfTermDoc> getL_docs()const{return l_docs;}

        void setFtc(int aux){ftc = aux;}
        int getFtc()const{return ftc;}

    private:
        // Frecuencia total del tÃÂ©rmino en la colecciÃÂ³n
        int ftc;

        /** Tabla Hash que se accederÃÂ¡ por el id del documento, devolviendo un
         * objeto de la clase InfTermDoc que contiene toda la informaciÃÂ³n de
         * apariciÃÂ³n del tÃÂ©rmino en el documento
         * */
        unordered_map<int, InfTermDoc> l_docs;
};



class InfDoc {
    friend ostream& operator<<(ostream& s, const InfDoc& p);

public:
    InfDoc (const InfDoc &);
    InfDoc (time_t,int, int, int, int, int);
    InfDoc();
    ~InfDoc ();
    InfDoc & operator= (const InfDoc &);

    //Auxiliares
    void setFechaModificacion(time_t aux){fechaModificacion = aux;}
    time_t getFechaModificacion()const{return fechaModificacion;}

    void setNumPal(int aux){numPal = aux;}
    int getNumPal()const{return numPal;}

    void setIdDoc(int aux){idDoc = aux;}
    int getIdDoc()const{return idDoc;}

    void setTamBytes(int aux){tamBytes = aux;}
    int getTamBytes()const{return tamBytes;}

    void setNumPalSinParada(int aux){numPalSinParada = aux;}
    int getNumPalSinParada()const{return numPalSinParada;}

    void setNumPalDiferentes(int aux){numPalDiferentes = aux;}
    int getNumPalDiferentes()const{return numPalDiferentes;}

private:
    // AÃÂ±adir cuantos mÃÂ©todos se consideren necesarios para manejar la parte privada de la clase

    // Identificador del documento. El primer documento indexado en la colecciÃÂ³n serÃÂ¡ el identificador 1
    int idDoc;

    // NÃÂº total de palabras del documento
    int numPal;

    // NÃÂº total de palabras sin stop-words del documento
    int numPalSinParada;

    // NÃÂº total de palabras diferentes que no sean stop-words (sin acumular la frecuencia de cada una de ellas)
    int numPalDiferentes;

    // TamaÃÂ±o en bytes del documento
    int tamBytes;

    /** Atributo correspondiente a la fecha y hora (completa) de modificaciÃÂ³n del documento.
     * El tipo Ã¢ÂÂFecha/horaÃ¢ÂÂ lo elegirÃÂ¡/implementarÃÂ¡ el alumno
     * */
    time_t fechaModificacion;
};

class InfColeccionDocs {
    friend ostream& operator<<(ostream& s, const InfColeccionDocs& p);

public:
    InfColeccionDocs (const InfColeccionDocs &);
    InfColeccionDocs (int,int,int,int,int);
    InfColeccionDocs ();
    ~InfColeccionDocs ();
    InfColeccionDocs & operator= (const InfColeccionDocs &);

    //Auxiliares
    void setNumDocs(int aux){numDocs = aux;}
    int getNumDocs()const{return numDocs;}

    void setNumTotalPal(int aux){numTotalPal = aux;}
    int getNumTotalPal()const{return numTotalPal;}

    void setNumTotalPalSinParada(int aux){numTotalPalSinParada = aux;}
    int getNumTotalPalSinParada()const{return numTotalPalSinParada;}

    void setNumTotalPalDiferentes(int aux){numTotalPalDiferentes = aux;}
    int getNumTotalPalDiferentes()const{return numTotalPalDiferentes;}

    void setTamBytes(int aux){tamBytes = aux;}
    int getTamBytes()const{return tamBytes;}

private:
    // AÃÂ±adir cuantos mÃÂ©todos se consideren necesarios para manejar la parte privada de la clase

    // NÃÂº total de documentos en la colecciÃÂ³n
    int numDocs;

    // NÃÂº total de palabras en la colecciÃÂ³n
    int numTotalPal;

    // NÃÂº total de palabras sin stop-words en la colecciÃÂ³n
    int numTotalPalSinParada;

    /** NÃÂº total de palabras diferentes en la colecciÃÂ³n que no sean
     * stopwords (sin acumular la frecuencia de cada una de ellas)
     * */
    int numTotalPalDiferentes;

    // TamaÃÂ±o total en bytes de la colecciÃÂ³n
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

    void nuevaPosicionTermino(int pos) {posTerm.push_back(pos);}

    //Seters/Geters
    void setFt(int aux){ft = aux;}
    int getFt()const{return ft;}

    void setPosTerm(list<int> aux){posTerm = aux;}
    list<int> getPosTerm()const{return posTerm;}

private:
    // AÃÂ±adir cuantos mÃÂ©todos se consideren necesarios para manejar la parte privada de la clase

    // Frecuencia total del tÃÂ©rmino en la pregunta
    int ft;

    /** Solo se almacenarÃÂ¡ esta informaciÃÂ³n si el campo privado del indexador almacenarPosTerm == true
     * Lista de nÃÂºmeros de palabra en los que aparece el tÃÂ©rmino en la pregunta.
     * Los nÃÂºmeros de palabra comenzarÃÂ¡n desde cero (la primera
     * palabra de la pregunta). Se numerarÃÂ¡n las palabras de parada. EstarÃÂ¡
     * ordenada de menor a mayor posiciÃÂ³n.
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
    int getNumTotalPal()const{return numTotalPal;}

    void setNumTotalPalSinParada(int aux){numTotalPalSinParada = aux;}
    int getNumTotalPalSinParada()const{return numTotalPalSinParada;}

    void setNumTotalPalDiferentes(int aux){numTotalPalDiferentes = aux;}
    int getNumTotalPalDiferentes()const{return numTotalPalDiferentes;}

    private:
        // AÃÂ±adir cuantos mÃÂ©todos se consideren necesarios para manejar la parte privada de la clase

        // NÃÂº total de palabras en la pregunta
        int numTotalPal;

        // NÃÂº total de palabras sin stop-words en la pregunta
        int numTotalPalSinParada;

        /* NÃÂº total de palabras diferentes en la pregunta que no sean stop-words
         * (sin acumular la frecuencia de cada una de ellas)
         * */
        int numTotalPalDiferentes;
};

#endif //PRACTICA_2_INDEXADORINFORMACION_H