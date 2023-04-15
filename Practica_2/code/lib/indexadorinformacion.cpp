#include "../include/indexadorinformacion.h"

/**
 * InfTermDoc
 */

InfTermDoc::InfTermDoc(const InfTermDoc& aux){
    ft = aux.ft;
    posTerm = aux.posTerm;
}

InfTermDoc::InfTermDoc(){
    ft = 0;
}

InfTermDoc::~InfTermDoc(){
    ft = 0;
    posTerm.clear();
}

InfTermDoc
&InfTermDoc::operator=(const InfTermDoc& aux){
    if (this == &aux) {
        return *this;
    }else{
        (*this).~InfTermDoc();

        ft = aux.ft;
        posTerm = aux.posTerm;

        return *this;
    }
}

ostream& operator<<(ostream& s, const InfTermDoc& p) {
    s << "ft: " << p.ft;
    /* A continuación se mostrarían todos los elementos de p.posTerm
     * (“posicion TAB posicion TAB ... posicion,
     * es decir nunca finalizará en un TAB”): s <<“\t“ << posicion;
     * */
    return s;
}

/**
 * InformacionTermino
 */

InformacionTermino::InformacionTermino(const InformacionTermino& aux){
    ftc = aux.ftc;
    l_docs = aux.l_docs;
}

InformacionTermino::InformacionTermino(){
    ftc = 0;
}

InformacionTermino::~InformacionTermino(){
    ftc = 0;
    l_docs.clear();
}

InformacionTermino
&InformacionTermino::operator=(const InformacionTermino& aux){
    if (this == &aux) {
        return *this;
    }else{
        (*this).~InformacionTermino();

        ftc = aux.ftc;
        l_docs = aux.l_docs;

        return *this;
    }
}

ostream& operator<<(ostream& s, const InformacionTermino& p) {
    s << "Frecuencia total: " << p.ftc << "\tfd: " << p.l_docs.size();
    /* A continuación se mostrarían todos los elementos de p.l_docs: s <<
     * “\tId.Doc: “ << idDoc << “\t” << InfTermDoc;
     * */
    return s;
}

/**
 * InfDoc
 */

InfDoc::InfDoc(const InfDoc& aux) {
    fechaModificacion = aux.fechaModificacion;
    numPal = aux.numPal;
    idDoc = aux.idDoc;
    numPalSinParada = aux.numPalSinParada;
    numPalDiferentes = aux.numPalDiferentes;
    tamBytes = aux.tamBytes;
}

InfDoc::InfDoc(time_t auxFechaModificacion,int auxNumPalDiferentres, int auxTamBytes, int auxNumPalSinParada,int auxIdDoc ,int auxNumPal){
    fechaModificacion = auxFechaModificacion;
    numPal = auxNumPal;
    idDoc = auxIdDoc;
    numPalSinParada = auxNumPalSinParada;
    numPalDiferentes = auxNumPalDiferentres;
    tamBytes = auxTamBytes;
}

InfDoc::InfDoc(){
    fechaModificacion = time(0);
    numPal = 0;
    idDoc = 0;
    numPalSinParada = 0;
    numPalDiferentes = 0;
    tamBytes = 0;
}

InfDoc::~InfDoc(){
    fechaModificacion = 0 ;
    numPal = 0;
    idDoc = 0;
    numPalSinParada = 0;
    numPalDiferentes = 0;
    tamBytes = 0;
}

InfDoc
&InfDoc::operator=(const InfDoc& aux){
    if (this == &aux) {
        return *this;
    }else{
        (*this).~InfDoc();

        idDoc = aux.idDoc;
        numPal = aux.numPal;
        numPalSinParada = aux.numPalSinParada;
        numPalDiferentes = aux.numPalDiferentes;
        tamBytes = aux.tamBytes;
        fechaModificacion = aux.fechaModificacion;

        return *this;
    }
}

ostream& operator<<(ostream& s, const InfDoc& p) {
    s << "idDoc: " << p.idDoc << "\tnumPal: " << p.numPal <<
      "\tnumPalSinParada: " << p.numPalSinParada << "\tnumPalDiferentes: " <<
      p.numPalDiferentes << "\ttamBytes: " << p.tamBytes;
    return s;
}

/**
 * InfColeccionDocs
 */


InfColeccionDocs::InfColeccionDocs(const InfColeccionDocs& aux) {
    tamBytes = aux.tamBytes;
    numTotalPalSinParada = aux.numTotalPalSinParada;
    numTotalPalDiferentes = aux.numTotalPalDiferentes;
    numDocs = aux.numDocs;
    numTotalPal = aux.numTotalPal;

}

InfColeccionDocs::InfColeccionDocs(int auxNumTotalPalDiferentes, int auxTamBytes, int auxNumTotalPalSinParada,int auxNumDocs ,int auxNumTotalPal){
    tamBytes = auxTamBytes;
    numTotalPalSinParada = auxNumTotalPalSinParada;
    numTotalPalDiferentes = auxNumTotalPalDiferentes;
    numTotalPal = auxNumTotalPal;
    numDocs = auxNumDocs;
}

InfColeccionDocs::InfColeccionDocs(){
    tamBytes = 0;
    numTotalPalSinParada = 0;
    numTotalPalDiferentes = 0;
    numTotalPal = 0;
    numDocs = 0;
}

InfColeccionDocs::~InfColeccionDocs(){
    tamBytes = 0;
    numTotalPalSinParada = 0;
    numTotalPalDiferentes = 0;
    numTotalPal = 0;
    numDocs = 0;
}

InfColeccionDocs
&InfColeccionDocs::operator=(const InfColeccionDocs& aux){
    if (this == &aux) {
        return *this;
    }else{
        (*this).~InfColeccionDocs();

        tamBytes = aux.tamBytes;
        numTotalPalSinParada = aux.numTotalPalSinParada;
        numTotalPalDiferentes = aux.numTotalPalDiferentes;
        numDocs = aux.numDocs;
        numTotalPal = aux.numTotalPal;

        return *this;
    }
}

ostream& operator<<(ostream& s, const InfColeccionDocs& p){
    s << "numDocs: " << p.numDocs << "\tnumTotalPal: " << p.numTotalPal <<
      "\tnumTotalPalSinParada: " << p.numTotalPalSinParada <<
      "\tnumTotalPalDiferentes: " << p.numTotalPalDiferentes << "\ttamBytes: " <<
      p.tamBytes;
    return s;
}

/**
* InformacionTerminoPregunta
*/

InformacionTerminoPregunta::InformacionTerminoPregunta(const InformacionTerminoPregunta& aux) {
    ft = aux.ft;
    posTerm = aux.posTerm;
}

InformacionTerminoPregunta::InformacionTerminoPregunta(){
    ft = 0;
}

InformacionTerminoPregunta::~InformacionTerminoPregunta(){
    ft = 0;
    posTerm.clear();
}

InformacionTerminoPregunta
&InformacionTerminoPregunta::operator=(const InformacionTerminoPregunta& aux){
    if (this == &aux) {
        return *this;
    }else{
        (*this).~InformacionTerminoPregunta();

        ft = aux.ft;
        posTerm = aux.posTerm;

        return *this;
    }
}

ostream& operator<<(ostream& s, const InformacionTerminoPregunta& p) {
    s << "ft: " << p.ft;
    // A continuación se mostrarían todos los elementos de p.posTerm (“posicion
    // TAB posicion TAB ... posicion, es decir nunca finalizará en un TAB”): s <<
    // “\t“ << posicion;
    return s;
}

/**
* InformacionPregunta
*/

InformacionPregunta::InformacionPregunta(const InformacionPregunta& aux) {
    numTotalPalSinParada = aux.numTotalPalSinParada;
    numTotalPal = aux.numTotalPal;
    numTotalPalDiferentes = aux.numTotalPalDiferentes;

}

InformacionPregunta::InformacionPregunta(){
    numTotalPalSinParada = 0;
    numTotalPal = 0;
    numTotalPalDiferentes = 0;
}

InformacionPregunta::~InformacionPregunta(){
    numTotalPalSinParada = 0;
    numTotalPal = 0;
    numTotalPalDiferentes = 0;
}

InformacionPregunta
&InformacionPregunta::operator=(const InformacionPregunta& aux){
    if (this == &aux) {
        return *this;
    }else{
        (*this).~InformacionPregunta();

        numTotalPalSinParada = aux.numTotalPalSinParada;
        numTotalPal = aux.numTotalPal;
        numTotalPalDiferentes = aux.numTotalPalDiferentes;

        return *this;
    }
}

ostream& operator<<(ostream& s, const InformacionPregunta& p){
    s << "numTotalPal: " << p.numTotalPal << "\tnumTotalPalSinParada: "<<
      p.numTotalPalSinParada << "\tnumTotalPalDiferentes: " << p.numTotalPalDiferentes;
    return s;
}