#include "../include/indexadorHash.h"

IndexadorHash::IndexadorHash(const string& fichStopWords, const string& delimitadores,const bool& detectComp,
                             const bool& minuscSinAcentos, const string&dirIndice, const int& tStemmer, const bool& almEnDisco,
                             const bool& almPosTerm){
    tok = Tokenizador(delimitadores,detectComp,minuscSinAcentos);

    almacenarEnDisco = almEnDisco;
    almacenarPosTerm = almPosTerm;
    ficheroStopWords = fichStopWords;

    if (dirIndice != "") {
        directorioIndice = dirIndice;
    } else {
        directorioIndice = "./";
    }

    if(tStemmer >= 0 && tStemmer <= 2){
        tipoStemmer = tStemmer;
    }else{
        cerr << "Error tStemmer" << endl;
    }

    RecogerInfoColecc();
    AlmacenarStopWords();
}

IndexadorHash::IndexadorHash(const string& directorioIndexacion){
    directorioIndice = directorioIndexacion;
    RecuperarIndexacion(directorioIndexacion);
}

IndexadorHash::IndexadorHash(const IndexadorHash& indx){
    indice = indx.indice;
    indiceDocs = indx.indiceDocs;

    informacionColeccionDocs = indx.informacionColeccionDocs;

    pregunta = indx.pregunta;
    indicePregunta = indx.indicePregunta;
    infPregunta = indx.infPregunta;

    stopWords = indx.stopWords;
    ficheroStopWords = indx.ficheroStopWords;

    tok = Tokenizador(indx.tok);

    directorioIndice = indx.directorioIndice;

    tipoStemmer = indx.tipoStemmer;

    almacenarEnDisco = indx.almacenarEnDisco;
    almacenarPosTerm = indx.almacenarPosTerm;
}

IndexadorHash::IndexadorHash(){
    informacionColeccionDocs.~InfColeccionDocs();
    infPregunta.~InformacionPregunta();

    auxDisco.clear();
    stopWords.clear();

    tok = Tokenizador();

    VaciarIndiceDocs();
    VaciarIndicePreg();

    ficheroStopWords = "";
    directorioIndice = "./";
    tipoStemmer = 0;
    almacenarEnDisco = false;
    almacenarPosTerm = false;
}

//*********
IndexadorHash::~IndexadorHash(){
    VaciarIndiceDocs();
    VaciarIndicePreg();

    auxDisco.clear();
    stopWords.clear();

    pregunta = ficheroStopWords = directorioIndice = "";
    almacenarEnDisco = almacenarPosTerm = false;
    tipoStemmer = 0;
}

IndexadorHash
&IndexadorHash::operator= (const IndexadorHash& indx){
    if (this == &indx) {
        return *this;
    }else{
        (*this).~IndexadorHash();
        IndexadorHash(indx);
        return *this;
    }
}

bool
IndexadorHash::Indexar(const string& ficheroDocumentos){

}

bool
IndexadorHash::IndexarDirectorio(const string& dirAIndexar){

}

bool
IndexadorHash::GuardarIndexacion() const{

}

bool
IndexadorHash::RecuperarIndexacion (const string& directorioIndexacion){

}

bool
IndexadorHash::IndexarPregunta(const string& preg){

}

bool
IndexadorHash::DevuelvePregunta(string& preg) const{

}

bool
IndexadorHash::DevuelvePregunta(const string& word, InformacionTerminoPregunta& inf) const{

}

bool
IndexadorHash::DevuelvePregunta(InformacionPregunta& inf) const{

}

bool
IndexadorHash::Devuelve(const string& word, InformacionTermino& inf) const{

}

bool
IndexadorHash::Devuelve(const string& word, const string& nomDoc, InfTermDoc& InfDoc) const{

}

bool
IndexadorHash::Existe(const string& word) const{

}

bool
IndexadorHash::Borra(const string& word){

}

bool
IndexadorHash::BorraDoc(const string& nomDoc){

}

void
IndexadorHash::VaciarIndiceDocs(){

}

void
IndexadorHash::VaciarIndicePreg(){

}

bool
IndexadorHash::Actualiza(const string& word, const InformacionTermino& inf){

}

bool
IndexadorHash::Inserta(const string& word, const InformacionTermino& inf){

}

int
IndexadorHash::NumPalIndexadas() const{

}

string
IndexadorHash::DevolverFichPalParada () const{

}

void
IndexadorHash::ListarPalParada() const{

}

int
IndexadorHash::NumPalParada() const{

}

string
IndexadorHash::DevolverDelimitadores () const{

}

bool
IndexadorHash::DevolverCasosEspeciales () const{

}

bool
IndexadorHash::DevolverPasarAminuscSinAcentos () const{

}

bool
IndexadorHash::DevolverAlmacenarPosTerm () const{

}

string
IndexadorHash::DevolverDirIndice () const{

}

int
IndexadorHash::DevolverTipoStemming () const{

}

bool
IndexadorHash::DevolverAlmEnDisco () const{

}

void
IndexadorHash::ListarInfColeccDocs() const{

}

void
IndexadorHash::ListarTerminos() const{

}

bool
IndexadorHash::ListarTerminos(const string& nomDoc) const{

}

void
IndexadorHash::ListarDocs() const{

}

bool
IndexadorHash::ListarDocs(const string& nomDoc) const{

}

//Auxiliares

void IndexadorHash::ObtenerInfo() {
    informacionColeccionDocs = InfColeccionDocs();

    int numPal = 0, numPalSinParada = 0, tamBytes = 0;
    for (const pair <string,InfDoc>& indc : indiceDocs) {
        numPal += indc.second.getNumPal();
        numPalSinParada += indc.second.getNumPalSinParada();
        tamBytes += indc.second.getTamBytes();
    }

    informacionColeccionDocs.setNumDocs(indiceDocs.size());
    informacionColeccionDocs.setNumTotalPal(numPal);
    informacionColeccionDocs.setNumTotalPalSinParada(numPalSinParada);
    informacionColeccionDocs.setNumTotalPalDiferentes(indice.size());
    informacionColeccionDocs.setTamBytes(tamBytes);
}