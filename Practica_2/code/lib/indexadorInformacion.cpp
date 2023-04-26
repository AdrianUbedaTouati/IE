#include "../include/indexadorInformacion.h"

/**
 * InformacionTermino
 */

ostream& operator<<(ostream& s, const InformacionTermino& p) {
    s << "Frecuencia total: " << p.ftc << "\tfd: " << p.l_docs.size();
    for (auto elem : p.l_docs)
    {
        s << "\t" << "Id.Doc: " << elem.first << "\t" << elem.second;
    }

    return s;
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

InformacionTermino::InformacionTermino(const InformacionTermino& aux){
    ftc = aux.ftc;
    l_docs = aux.l_docs;
}

InformacionTermino::InformacionTermino ()
{
    ftc = 0;
}

InformacionTermino::~InformacionTermino(){
    ftc = 0;
    l_docs.clear();
}

void
InformacionTermino::suprimirDocumento (const int& idDoc)
{
    auto posicion = l_docs.find(idDoc);
    ftc = ftc - posicion->second.getFt();
    l_docs.erase(idDoc);
}

void
InformacionTermino::incrementarFrecuencia (const int& id, const int& posicion = 1)
{
    ftc++;
    if (posicion == -1){
        return;
    }else{
        auto insercionDoc = l_docs.insert(pair<int, InfTermDoc>(id, InfTermDoc())).first;
        insercionDoc->second.aumentoFrecuencia(posicion);
    }
}

/**
 * InfTermDoc
 */

ostream& operator<<(ostream& s, const InfTermDoc& p) {
    s << "ft: " << p.ft;
    for (auto elem : p.posTerm)
    {
        s << "\t" << elem;
    }
    return s;
}

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

/**
 * InfDoc
 */

ostream& operator<<(ostream& s, const InfDoc& p) {
    s << "idDoc: " << p.idDoc << "\t" << "numPal: " << p.numPal <<
      "\t" << "numPalSinParada: " << p.numPalSinParada << "\t" << "numPalDiferentes: " <<
      p.numPalDiferentes << "\t" <<"tamBytes: " << p.tamBytes;
    return s;
}

InfDoc::InfDoc(const InfDoc& aux) {
    fechaModificacion = aux.fechaModificacion;
    numPal = aux.numPal;
    idDoc = aux.idDoc;
    numPalSinParada = aux.numPalSinParada;
    numPalDiferentes = aux.numPalDiferentes;
    tamBytes = aux.tamBytes;
}

InfDoc::InfDoc (int& aux)
{
    fechaModificacion = time(0);
    numPal = 0;
    idDoc = aux;
    numPalSinParada = 0;
    numPalDiferentes = 0;
    tamBytes = 0;
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

/**
 * InfColeccionDocs
 */

ostream& operator<< (ostream& s, const InfColeccionDocs& p)
{
    s << p.ToString();

    return s;
}

string
InfColeccionDocs::ToString () const
{
    return  "numDocs: " + to_string(numDocs) + "\tnumTotalPal: " + to_string(numTotalPal) + "\tnumTotalPalSinParada: " + to_string(numTotalPalSinParada) + "\tnumTotalPalDiferentes: " + to_string(numTotalPalDiferentes) + "\ttamBytes: " + to_string(tamBytes);
}

InfColeccionDocs::InfColeccionDocs(const InfColeccionDocs& aux) {
    tamBytes = aux.tamBytes;
    numTotalPalSinParada = aux.numTotalPalSinParada;
    numTotalPalDiferentes = aux.numTotalPalDiferentes;
    numDocs = aux.numDocs;
    numTotalPal = aux.numTotalPal;

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

/**
* InformacionTerminoPregunta
*/

ostream& operator<<(ostream& s, const InformacionTerminoPregunta& p) {
    s << "ft: " << p.ft;
    for(auto elem : p.posTerm){
        s << "\t" << elem;
    }
    return s;
}

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

/**
* InformacionPregunta
*/

ostream& operator<<(ostream& s, const InformacionPregunta& p){
    s << "numTotalPal: " << p.numTotalPal << "\tnumTotalPalSinParada: "<<
      p.numTotalPalSinParada << "\tnumTotalPalDiferentes: " << p.numTotalPalDiferentes;
    return s;
}

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