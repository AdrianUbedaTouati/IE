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
    int ft;
    list<int> posTerm;

public:
    InfTermDoc (const InfTermDoc&);
    InfTermDoc ();
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

    int ftc;
    unordered_map<int, InfTermDoc> l_docs;

public:
    InformacionTermino (const InformacionTermino&);
    InformacionTermino ();
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
    int idDoc;
    int numPal;
    int numPalSinParada;
    int numPalDiferentes;
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
    int numDocs;
    int  numTotalPal;
    int numTotalPalSinParada;
    int numTotalPalDiferentes;
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
    int ft;
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
    int numTotalPal;
    int numTotalPalSinParada;
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