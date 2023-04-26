#ifndef PRACTICA_2_INDEXADORINFORMACION_H
#define PRACTICA_2_INDEXADORINFORMACION_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <list>
#include <unordered_map>
#include <ctime>

using namespace std;

class InfTermDoc
{
friend ostream& operator<< (ostream&, const InfTermDoc&);

private:
    // Frecuencia del término en el documento
    int ft;

    /* Solo se almacenará esta información si el campo privado del indexador
    almacenar PosTerm == true
    Lista de números de palabra en los que aparece el término en el
    documento. Los números de palabra comenzarán desde cero (la primera
    palabra del documento). Se numerarán las palabras de parada. Estará
    ordenada de menor a mayor posición.
    */
    list<int> posTerm;

public:
    InfTermDoc (const InfTermDoc&);

    // Inicializa ft = 0;
    InfTermDoc ();

    // Pone ft = 0;
    ~InfTermDoc ();

    InfTermDoc& operator= (const InfTermDoc&);

    //auxiliares metodos

    void aumentoFrecuencia (const int& aux){ft++;posTerm.push_back(aux);}

    int getFt () const{return ft;}
    void setFt (int &aux){ft = aux;}
};

class InformacionTermino
{
friend ostream& operator<< (ostream&, const InformacionTermino&);
private:
    // Frecuencia total del término en la colección
    int ftc;

    /* Tabla Hash que se accederá por el id del documento, devolviendo un
     * objeto de la clase InfTermDoc que contiene toda la información de
     * aparición del término en el documento
     * */
    unordered_map<int, InfTermDoc> l_docs;

public:
    InformacionTermino (const InformacionTermino&);

    // Inicializa ftc = 0
    InformacionTermino ();

    // Pone ftc = 0 y vacia l_docs
    ~InformacionTermino ();

    InformacionTermino& operator= (const InformacionTermino&);

    //auxiliares metodos

    bool contieneEnDoc(const int& id) const{
        return  l_docs.cend() != l_docs.find(id);
    }

    void suprimirDocumento (const int&);
    void incrementarFrecuencia (const int&, const int&);

    InfTermDoc getInfTermDoc (const int& id) const{return l_docs.at(id);}

    auto getL_docs () const{return l_docs;}
    void setL_docs (unordered_map<int,InfTermDoc> &aux){l_docs = aux;}
};

class InfDoc
{
friend ostream& operator<< (ostream&, const InfDoc&);
private:
    /* Identificador del documento. El primer documento indexado en la
        colección será el identificador 1
    */
    int idDoc;

    // Nº total de palabras del documento
    int numPal;

    // Nº total de palabras sin stop-words del documento
    int numPalSinParada;

    /* Nº total de palabras diferentes que no sean stop-words (sin acumular
        la frecuencia de cada una de ellas)
    */
    int numPalDiferentes;

    // Tamaño en bytes del documento
    int tamBytes;

    time_t fechaModificacion;

public:
    InfDoc (const InfDoc&);
    InfDoc ();
    InfDoc (int&);
    ~InfDoc ();
    InfDoc& operator= (const InfDoc&);

    //auxiliares metodos

    int getIdDoc () const{return idDoc;}
    void setIdDoc (const int& aux){idDoc = aux;}

    int getNumPalDiferentes () const{return numPalDiferentes;}
    void setNumPalDiferentes (const int& aux){numPalDiferentes = aux;}

    time_t getFechaModificacion () const{return fechaModificacion;}
    void setFechaModificacion (const time_t& aux){fechaModificacion = aux;}

    int getTamBytes () const{return tamBytes;}
    void setTamBytes (const int& aux){tamBytes = aux;}

    int getNumPal () const{return numPal;}
    void setNumPal (const int& aux){numPal = aux;}

    int getNumPalSinParada () const{return numPalSinParada;}
    void getNumPalSinParada (const int& aux){numPalSinParada = aux;}

    void aumentoNumPalSinParada (const int& aux){numPalSinParada += aux;}
    void aumentoNumPalDiferentes (const int& aux){numPalDiferentes += aux;}
};

class InfColeccionDocs
{
friend ostream& operator<< (ostream&, InfColeccionDocs&);
private:
    // Nº total de documentos en la colección
    int numDocs;

    // Nº total de palabras en la colección
    int  numTotalPal;

    // Nº total de palabras sin stop-words en la colección
    int numTotalPalSinParada;

    /* Nº total de palabras diferentes en la colección que no sean
        stop-words (sin acumular la frecuencia de cada una de ellas)
    */
    int numTotalPalDiferentes;

    // Tamaño total en bytes de la colección
    int tamBytes;

public:
    InfColeccionDocs (const InfColeccionDocs&);
    InfColeccionDocs ();
    ~InfColeccionDocs ();
    InfColeccionDocs& operator= (const InfColeccionDocs&);

    //auxiliares metodos

    string ToString () const;
    int getNumDocs () const{return numDocs;}

    void aumentoNumTotalPalSinParada (const int& aux){ numTotalPalSinParada += aux;}
    void aumentoNumTotalPalDiferentes (const int& aux){ numTotalPalDiferentes += aux;}
    void aumentoNumDocs (const int& aux){ numDocs += aux;}
    void aumentoNumTotalPal (const int& aux){ numTotalPal += aux;}
    void aumentoTamBytes (const int& aux){ tamBytes += aux;}

    void setNumTotalPalDiferentes (const int& aux){numTotalPalDiferentes = aux;}
};

class InformacionTerminoPregunta
{
friend ostream& operator<< (ostream&, const InformacionTerminoPregunta&);
private:
    // Frecuencia total del término en la pregunta
    int ft;
    /* Solo se almacenará esta información si el campo privado del indexador
        almacenarPostTerm == true
      Lista de números de palabra en los que aparece el término en la
        pregunta. Los números de palabra comenzarán desde cero (la primera
        palabra de la pregunta). Se numerarán las palabras de parada. Estará
        ordenada de menor a mayor posición.
    */
    list<int> posTerm;
public:
    InformacionTerminoPregunta (const InformacionTerminoPregunta&);
    InformacionTerminoPregunta ();
    ~InformacionTerminoPregunta ();
    InformacionTerminoPregunta& operator= (const InformacionTerminoPregunta&);

    //auxiliares metodos

    void incrementarFrecuencia (const int& aux){
        {
            ft++;
            if (aux == -1) {
                return;
            }else{
                posTerm.push_back(aux);
            }
        }
    }
};

class InformacionPregunta
{
friend ostream& operator<< (ostream&, const InformacionPregunta&);
private:

    // Nº total de palabras en la pregunta
    int numTotalPal;

    // Nº total de palabras sin stop-words en la pregunta
    int numTotalPalSinParada;

    /* Nº total de palabras diferentes en la pregunta que no sean stop-words
        (sin acumular la frecuencia de cada una de ellas)
    */
    int numTotalPalDiferentes;

public:
    InformacionPregunta (const InformacionPregunta&);
    InformacionPregunta ();
    ~InformacionPregunta ();
    InformacionPregunta& operator= (const InformacionPregunta&);

    //auxiliares metodos
    void aumentoNumTotalPalSinParada (const int& aux){ numTotalPalSinParada += aux;}
    void aumentoNumTotalPalDiferentes (const int& aux){ numTotalPalDiferentes += aux;}
    void aumentoNumTotalPal (const int& aux){ numTotalPal += aux;}
};

#endif //PRACTICA_2_INDEXADORINFORMACION_H