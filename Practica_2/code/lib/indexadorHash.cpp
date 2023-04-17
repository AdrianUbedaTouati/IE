#include "../include/indexadorHash.h"
#include "../include/stemmer.h"

IndexadorHash::IndexadorHash(const string& fichStopWords, const string& delimitadores,const bool& detectComp,
                             const bool& minuscSinAcentos, const string&dirIndice, const int& tStemmer, const bool& almEnDisco,
                             const bool& almPosTerm){
    tok = Tokenizador(delimitadores,detectComp,minuscSinAcentos);

    almacenarEnDisco = almEnDisco;
    almacenarPosTerm = almPosTerm;
    ficheroStopWords = fichStopWords;

    ObtenerInfo();

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

    fstream i;
    string line;
    i.open(ficheroStopWords,ios::in);
    if (i) {
        while (!i.eof()) {
            i >> line;
            if (line != "" && stopWords.find(line) == stopWords.end()) {
                stopWords.insert(line);
            }
        }
        i.close();
    }else{
        cerr << "ERROR: " << ficheroStopWords << " fichero no encontrado.\n";
        return;
    }


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
        return *this;
    }
}

bool
IndexadorHash::Indexar(const string& ficheroDocumentos){
    fstream infile;
    list<string> tokens;

    infile.open(ficheroDocumentos, ios::in);
    if(!infile) {
        cerr << "ERROR: " << ficheroDocumentos << " fichero no encontrado.\n";
        return false;
    }

    while(!infile.eof()) {
        string doc, line;
        infile >> doc;
        if (doc != "") {
            fstream infile2;
            infile2.open(doc, ios::in);
            if(!infile2) {
                cerr << "ERROR: " << doc << " fichero no encontrado.\n";
                return false;
            }

            tokens.clear();
            while (getline(infile2, line)) {
                if (line != "") {
                    tok.Tokenizar(line, tokens);
                    int id;
                    // Si el documento ya ha sido indexado
                    if (indiceDocs.find(doc) != indiceDocs.end()) {
                        time_t t = indiceDocs.at(doc).getFechaModificacion();

                        // Si la fecha actual es mas reciente que la almacenada
                        if (time(0) > t) {
                            id = indiceDocs.at(doc).getIdDoc();
                            BorraDoc(doc);
                        }
                    } else {
                        id = indiceDocs.size() + 1;
                    }
                    //
                    stemmerPorter stem;
                    int numPalDif = 0, numPalSinPar = 0, numPal = 0;

                    for (const auto& token : tokens) {
                        string t = token;
                        stem.stemmer(t, tipoStemmer);
                        // Si no es palabra de parada
                        if (stopWords.find(t) == stopWords.end()) {
                            ++numPalSinPar;
                            InformacionTermino term;
                            InfTermDoc termDoc;
                            if (indice.find(t) == indice.end()) {
                                ++numPalDif;
                                term.aumentoFtc();
                                termDoc.AumentoFt();
                                if (almacenarPosTerm) {
                                    termDoc.nuevoPosTerm(numPal);
                                }
                                term.nuevoL_docs(id, termDoc);
                                try {
                                    indice.insert(pair<string, InformacionTermino>(t, term));
                                } catch (const bad_alloc &e) {
                                    cerr << "ERROR: Falta de memoria principal. ";
                                    if (!almacenarEnDisco) {
                                        cerr << "Ultimo documento indexado: " << indiceDocs.end()->first
                                             << " Ultimo termino indexado: " << indice.end()->first << "\n";
                                        break;
                                    } else {
                                        cerr << "Ultimo termino indexado: " << auxDisco.end()->first << '\n';
                                        AlmacenarEnDisco(t, term);
                                    }
                                }
                            } else {
                                term = indice.at(t);
                                term.aumentoFtc();
                                // Si el termino ya se habia encontrado en este documento
                                if (term.getL_docs().find(id) != term.getL_docs().end()) {
                                    termDoc = term.getL_docs().at(id);
                                    termDoc.AumentoFt();
                                    if (almacenarPosTerm) {
                                        termDoc.nuevoPosTerm(numPal);
                                    }
                                } else {
                                    ++numPalDif;
                                    termDoc.AumentoFt();
                                    if (almacenarPosTerm) {
                                        termDoc.nuevoPosTerm(numPal);
                                    }
                                    term.nuevoL_docs(id, termDoc);
                                }
                            }
                        }
                        ++numPal;
                    }
                    struct stat buf;
                    stat(doc.c_str(), &buf);
                    indiceDocs.insert(pair<string, InfDoc> (doc, InfDoc(id, numPal, numPalSinPar, numPalDif, (int) *&buf.st_size, time(0))));
                    //

                    ObtenerInfo();
                }
            }
            infile2.close();
        }
    }
    return false;
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

void IndexadorHash::VaciarIndiceDocs() {
    indiceDocs.clear();
    indice.clear();
}

void IndexadorHash::VaciarIndicePreg() {
    indicePregunta.clear();
}

bool
IndexadorHash::Actualiza(const string& word, const InformacionTermino& inf){

}

bool IndexadorHash::Inserta(const string& word, const InformacionTermino& inf) {
    return indice.insert(pair<string, InformacionTermino>(word, inf)).second;
}

int IndexadorHash::NumPalIndexadas() const {
    return indice.size();
}

std::string IndexadorHash::DevolverFichPalParada() const {
    return ficheroStopWords;
}

void IndexadorHash::ListarPalParada() const {
    fstream infile;
    infile.open(ficheroStopWords, ios::in);
    if (!infile) {
        cerr << "ERROR: No existe el fichero: " << ficheroStopWords << "\n";
    }

    while(!infile.eof()){
        string line;
        infile >> line;
        cout << line << '\n';
    }

    infile.close();
}

int
IndexadorHash::NumPalParada() const {
    return stopWords.size();
}

string
IndexadorHash::DevolverDelimitadores() const {
    return tok.DelimitadoresPalabra();
}

bool
IndexadorHash::DevolverCasosEspeciales() const {
    return tok.CasosEspeciales();
}

bool
IndexadorHash::DevolverPasarAminuscSinAcentos() const {
    return tok.PasarAminuscSinAcentos();
}

string
IndexadorHash::DevolverDirIndice() const {
    return directorioIndice;
}

int
IndexadorHash::DevolverTipoStemming() const {
    return tipoStemmer;
}

bool
IndexadorHash::DevolverAlmacenarPosTerm() const {
    return almacenarPosTerm;
}

bool
IndexadorHash::DevolverAlmEnDisco() const {
    return almacenarEnDisco;
}

void
IndexadorHash::ListarInfColeccDocs() const {
    cout << informacionColeccionDocs << '\n';
}

void
IndexadorHash::ListarTerminos() const {
    for (const auto& termino : indice){
        cout << termino.first << '\t' << termino.second << '\n';
    }
}

bool
IndexadorHash::ListarTerminos(const string& nomDoc) const {
    try {
        auto doc = indiceDocs.at(nomDoc);
        for(const auto& termino : indice){
            cout << termino.first << '\t'
                 << termino.second.getL_docs().at(doc.getIdDoc()) << '\n';
        }
        return true;
    } catch (const out_of_range& e){
        return false;
    }
}

void
IndexadorHash::ListarDocs() const {
    for(const auto& doc : indiceDocs){
        cout << doc.first << '\t' << doc.second << '\n';
    }
}

bool
IndexadorHash::ListarDocs(const string &nomDoc) const {
    try {
        InfDoc doc = indiceDocs.at(nomDoc);
        cout << nomDoc << '\t' << doc << '\n';
        return true;
    } catch (const out_of_range& e){
        return false;
    }
}

//Auxiliares

void
IndexadorHash::ObtenerInfo() {
    int auxNumPal = 0;
    int auxNumPalSinParada = 0;
    int auxTamBytes = 0;

    for (const pair <string,InfDoc>& indc : indiceDocs) {
        auxNumPal += indc.second.getNumPal();
        auxNumPalSinParada += indc.second.getNumPalSinParada();
        auxTamBytes += indc.second.getTamBytes();
    }

    informacionColeccionDocs = InfColeccionDocs();

    informacionColeccionDocs.setNumDocs(indiceDocs.size());
    informacionColeccionDocs.setNumTotalPal(auxNumPal);
    informacionColeccionDocs.setTamBytes(auxTamBytes);
    informacionColeccionDocs.setNumTotalPalSinParada(auxNumPalSinParada);
    informacionColeccionDocs.setNumTotalPalDiferentes(indice.size());
}

void
IndexadorHash::GuardarPalabrasParada(){

}