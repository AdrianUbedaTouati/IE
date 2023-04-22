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
        cerr << "Error tStemmer \n";
    }


    string linea = "";
    fstream i;
    i.open(ficheroStopWords,ios::in);
    if (i) {
        while (!i.eof()) {
            i >> linea;
            if (linea != "" && stopWords.find(linea) == stopWords.end()) {
                stopWords.insert(linea);
            }
        }
        i.close();
    }else{
        cerr << "Error " << ficheroStopWords << " txt no existe.\n";
        i.close();
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

    RestablecerDocsIndice();
    RestablecerPregIndice();

    almacenarEnDisco = false;
    almacenarPosTerm = false;

    ficheroStopWords = "";
    directorioIndice = "./";

    tipoStemmer = 0;

}

//*********
IndexadorHash::~IndexadorHash(){
    RestablecerDocsIndice();
    RestablecerPregIndice();

    almacenarPosTerm = false;
    almacenarEnDisco = false;

    auxDisco.clear();
    stopWords.clear();

    ficheroStopWords ="";
    directorioIndice = "";
    pregunta ="";

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

        RestablecerDocsIndice();
        RestablecerPregIndice();

        ficheroStopWords = "";
        directorioIndice = "./";
        tipoStemmer = 0;
        almacenarEnDisco = false;
        almacenarPosTerm = false;
        return *this;
    }
}


bool IndexadorHash::Indexar(const string& ficheroDocumentos) {
    fstream infile;
    list<string> tokens;
//carpeta
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
                    IndexarDocumento(doc, id, tokens);
                    ObtenerInfo();
                }
            }
            infile2.close();
        }
    }
    return true;
}

bool IndexadorHash::IndexarDirectorio(const string& dirAIndexar) {
    struct stat dir;
    int err = stat(dirAIndexar.c_str(), &dir);
    if (err == -1 || !S_ISDIR(dir.st_mode)) {
        return false;
    } else {
        string cmd = "find " + dirAIndexar + "/ -follow | sort | awk '!/\\/$/{print}' > .lista_fich";
        system(cmd.c_str());
        return Indexar(".lista_fich");
    }
}

bool IndexadorHash::GuardarIndexacion() const {
    fstream outfile;
    outfile.open(directorioIndice, ios::out);
    outfile << "indexadorHash{\n"
            << "\tstemmer: " << tipoStemmer << "\n"
            << "\talmacenarEnDisco: " << boolalpha << almacenarEnDisco << "\n"
            << "\talmacenarPosTerm: " << boolalpha << almacenarPosTerm << "\n"
            << "\tpregunta: " << pregunta << "#FINP#\n"
            << "\tficheroStopWords: " << ficheroStopWords << "\n"

            << "\ttokenizador{\n"
            << "\t\tpasarAminuscSinAcentos: " << boolalpha << tok.PasarAminuscSinAcentos() << "\n"
            << "\t\tcasosEspeciales: " << boolalpha << tok.CasosEspeciales() << "\n"
            << "\t\tdelimitadoresPalabra: " << tok.DelimitadoresPalabra() << "#FIND#\n\t}\n"

            << "\tInfColeccionDocs{\n"
            << "\t\tnumDocs: " << informacionColeccionDocs.getNumDocs() << "\n"
            << "\t\tnumTotalPal: " << informacionColeccionDocs.getNumTotalPal() << "\n"
            << "\t\tnumTotalPalSinParada: " << informacionColeccionDocs.getNumTotalPalSinParada() << "\n"
            << "\t\tnumTotalPalDiferentes: " << informacionColeccionDocs.getNumTotalPalDiferentes() << "\n"
            << "\t\ttamBytes: " << informacionColeccionDocs.getTamBytes() << "\n\t}\n"

            << "\tInfDoc{ doc idDoc numPal numPalSinParada numPalDiferentes tamBytes fechaMod\n";
    for (const auto& doc : indiceDocs) {
        outfile << "\t\t" << doc.first
                << " " << doc.second.getIdDoc()
                << " " << doc.second.getNumPal()
                << " " << doc.second.getNumPalSinParada()
                << " " << doc.second.getNumPalDiferentes()
                << " " << doc.second.getTamBytes()
                << " " << doc.second.getFechaModificacion() << "\n";
    }
    outfile << "\t}\n"

            << "\tInfPregunta{\n"
            << "\t\tnumTotalPal: " << infPregunta.getNumTotalPal() << "\n"
            << "\t\tnumTotalPalSinParada: " << infPregunta.getNumTotalPalSinParada() << "\n"
            << "\t\tnumTotalPalDiferentes: " << infPregunta.getNumTotalPalDiferentes() << "\n"
            << "\t{\n"

            << "\tInormacionTermino{\n";
    for (const auto& term : indice) {
        outfile << "\t\t" << term.first
                << "," << term.second.getFtc() << ",";
        for (const auto& doc : term.second.getL_docs()) {
            outfile << doc.first << "/"
                    << doc.second.getFt() << "/";
            for (int pos : doc.second.getPosTerm()) {
                outfile << pos << "|";
            }
            outfile << " ";
        }
        outfile << "\n";
    }
    outfile << "\t}\n"
            << "\tInformacionTerminoPregunta(term,ft,posTerm posTerm ...){\n";
    for (const auto& term : indicePregunta) {
        outfile << "\t\t" << term.first
                << "," << term.second.getFt() << ",";
        for (const auto& pos : term.second.getPosTerm()) {
            outfile << pos << " ";
        }
        outfile << "\n";
    }
    outfile << "\t}\n}";

    bool ret = outfile.good();
    outfile.close();
    return ret;
}

bool IndexadorHash::hasEnding(const string& str, const string& ending) {
    if (str.length() >= ending.length()) {
        return (0 == str.compare (str.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

bool IndexadorHash::RecuperarIndexacion(const string& directorioIndexacion) {
    fstream infile;
    infile.open(directorioIndexacion, ios::in);
    if (!infile) {
        return false;
    }
    string line, section;
    string delimitadoresPalabra;
    bool pasarAminuscSinAcentos = true, casosEspeciales = true;
    while (getline(infile, line)) {
        if (line.rfind("\tstemmer:", 0) == 0) {
            tipoStemmer = (int)line.at(10) - '0';
        } else if (line.rfind("\talmacenarEnDisco:", 0) == 0) {
            istringstream(line.substr(19)) >> boolalpha >> almacenarEnDisco;
        } else if (line.rfind("\talmacenarPosTerm:", 0) == 0) {
            istringstream(line.substr(19)) >> boolalpha >> almacenarEnDisco;
        } else if (line.rfind("\tpregunta:", 0) == 0) {
            string aux = line.substr(11);
            // remove "#FINP#\n"
            if (hasEnding(line, "#FINP#\n")) {
                aux.resize(aux.size() - 7);
            }
            pregunta = aux;
        } else if (hasEnding(line, "#FINP#\n") && line.size() > 7) {
            // remove "#FINP#\n"
            if (hasEnding(line, "#FINP#\n")) {
                line.resize(line.size() - 7);
            }
            pregunta += line;
        } else if (line.rfind("\tficheroStopWords:", 0) == 0) {
            string aux = line.substr(19);
            // remove last char '\n'
            aux.pop_back();
            pregunta = aux;
        } // if section
        else if (line.at(0) == '\t' && line.at(1) != '\t') {
            section = line.substr(1);
        } else if (section.rfind("tokenizador{", 0) == 0) {
            if (line.rfind("\t\tpasarAminuscSinAcentos:", 0) == 0 && line.size() > 27) {
                bool b;
                istringstream(line.substr(26)) >> boolalpha >> b;
                pasarAminuscSinAcentos = b;
            } else if (line.rfind("\t\tcasosEspeciales:", 0) == 0 && line.size() > 20) {
                bool b;
                istringstream(line.substr(19)) >> boolalpha >> b;
                casosEspeciales = b;
            } else if (line.rfind("\t\tdelimitadoresPalabra:", 0) == 0) {
                string aux = line.substr(24);
                // remove "#FIND#\n"
                if (hasEnding(line, "#FIND#\n")) {
                    aux.resize(aux.size() - 7);
                }
                delimitadoresPalabra = aux;
            } else if (hasEnding(line, "#FIND#\n") && line.size() > 7) {
                // remove "#FIND#\n"
                if (hasEnding(line, "#FIND#\n")) {
                    line.resize(line.size() - 7);
                }
                delimitadoresPalabra += line;
            }
        } else if (section.rfind("InfColeccionDocs{", 0) == 0) {
            if (line.rfind("\t\tnumDocs:", 0) == 0) {
                informacionColeccionDocs.setNumDocs(stoi(line.substr(11)));
            } else if (line.rfind("\t\tnumTotalPal:", 0) == 0) {
                informacionColeccionDocs.setNumTotalPal(stoi(line.substr(15)));
            } else if (line.rfind("\t\tnumTotalPalSinParada:", 0) == 0) {
                informacionColeccionDocs.setNumTotalPalSinParada(stoi(line.substr(24)));
            } else if (line.rfind("\t\tnumTotalPalDiferentes:", 0) == 0) {
                informacionColeccionDocs.setNumTotalPalDiferentes(stoi(line.substr(25)));
            } else if (line.rfind("\t\ttamBytes:", 0) == 0) {
                informacionColeccionDocs.setTamBytes(stoi(line.substr(12)));
            }
        } else if (section.rfind("InfDoc{", 0) == 0) {
            // remove "\t\t" from the beginning
            line.erase(0,2);

            stringstream ss(line);
            string token;
            vector<string> tokens;
            while (ss >> token) {
                tokens.push_back(token);
            }
            InfDoc doc;
            doc.setIdDoc(stoi(tokens[1]));
            doc.setNumPal(stoi(tokens[2]));
            doc.setNumPalSinParada(stoi(tokens[3]));
            doc.setNumPalDiferentes(stoi(tokens[4]));
            doc.setTamBytes(stoi(tokens[5]));
            doc.setFechaModificacion(stoi(tokens[6]));
            indiceDocs[tokens[0]] = doc;
        } else if (section.rfind("InfPregunta{", 0) == 0) {
            if (line.rfind("\t\tnumTotalPal:", 0) == 0) {
                infPregunta.setNumTotalPal(stoi(line.substr(15)));
            } else if (line.rfind("\t\tnumTotalPalSinParada:", 0) == 0) {
                infPregunta.setNumTotalPalSinParada(stoi(line.substr(24)));
            } else if (line.rfind("\t\tnumTotalPalDiferentes:", 0) == 0) {
                infPregunta.setNumTotalPalDiferentes(stoi(line.substr(25)));
            }
        } else if (section.rfind("InformacionTermino{", 0) == 0) {
            // remove "\t\t" from the beginning
            line.erase(0,2);

            stringstream ss(line);
            string token;
            vector<string> tokens;
            while (getline(ss, token, ',')) {
                tokens.push_back(token);
            }
            InformacionTermino termino;
            termino.setFtc(stoi(tokens[1]));
            // store l_docs
            for (int i = 2; i < tokens.size(); ++i) {
                InfTermDoc infTermDoc;
                stringstream ss1(tokens[i]);
                string tDoc;
                vector<string> tDocs;
                while (getline(ss1, tDoc, '/')) {
                    tDocs.push_back(tDoc);
                }
                infTermDoc.setFt(stoi(tDocs[1]));
                // store
                if (almacenarPosTerm) {
                    stringstream ss2(tDocs[2]);
                    string pos;
                    while (getline(ss2, pos, '|')) {
                        infTermDoc.nuevaPosicionTermino(stoi(pos));
                    }
                }
                termino.nuevoL_docs(stoi(tDocs[0]), infTermDoc);
            }
            indice[tokens[0]] = termino;
        } else if (section.rfind("InformacionTerminoPregunta{", 0) == 0) {
            // remove "\t\t" from the beginning
            line.erase(0,2);

            stringstream ss(line);
            string token;
            vector<string> tokens;
            while (getline(ss, token, ',')) {
                tokens.push_back(token);
            }
            InformacionTerminoPregunta infTP;
            infTP.setFt(stoi(tokens[1]));
            stringstream ss1(tokens[2]);
            string pos;
            while (getline(ss1, pos, ' ')) {
                infTP.nuevaPosicionTermino(stoi(pos));
            }
            indicePregunta[tokens[0]] = infTP;
        }
    }
    tok = Tokenizador(delimitadoresPalabra, casosEspeciales, pasarAminuscSinAcentos);
    infile.close();
    return true;
}

bool IndexadorHash::IndexarPregunta(const string& preg) {
    pregunta = preg;
    list<string> tokens;
    RestablecerPregIndice();


    infPregunta = InformacionPregunta();
    stemmerPorter stem;
    int numPalSinPar = 0, numPal = 0;

    tok.Tokenizar(pregunta, tokens);
    for (const auto& token : tokens) {
        string t = token;
        stem.stemmer(t, tipoStemmer);
        // Si no es palabra de parada
        if (stopWords.find(token) == stopWords.end()) {
            ++numPalSinPar;
            InformacionTerminoPregunta termPreg;
            if (indicePregunta.find(token) != indicePregunta.end()) {
                termPreg = indicePregunta.at(token);
            } else {
                indicePregunta.insert(pair<string, InformacionTerminoPregunta>(token, termPreg));
            }
            termPreg.aumentoFt();
            if (almacenarPosTerm) {
                termPreg.nuevaPosicionTermino(numPal);
            }
            indicePregunta.at(token) = termPreg;
        }
        ++numPal;
    }
    infPregunta.setNumTotalPal(numPal);
    infPregunta.setNumTotalPalSinParada(numPalSinPar);
    infPregunta.setNumTotalPalDiferentes(indicePregunta.size());
    return true;
}

bool IndexadorHash::DevuelvePregunta(string& preg) const {
    if (pregunta != "" && !indicePregunta.empty()) {
        preg = pregunta;
        return true;
    }
    return false;
}

bool IndexadorHash::DevuelvePregunta(string& word, InformacionTerminoPregunta& inf) {
    stemmerPorter stem;
    string w = word;
    tok.Tokenizar(word);
    stem.stemmer(w, tipoStemmer);

    if (pregunta != "") {
        unordered_map<string, InformacionTerminoPregunta>::const_iterator i = indicePregunta.find(w);
        if (i != indicePregunta.end()) {
            inf= i->second;
            return true;
        }
    }
    inf.~InformacionTerminoPregunta();
    return false;
}

bool IndexadorHash::DevuelvePregunta(InformacionPregunta& inf) const {
    if (pregunta != "") {
        inf = infPregunta;
        return true;
    }
    inf.~InformacionPregunta();
    return false;
}

bool IndexadorHash::Devuelve(string& word, InformacionTermino& inf) {
    stemmerPorter stem;
    string w = word;
    tok.Tokenizar(word);
    stem.stemmer(w, tipoStemmer);

    unordered_map<string, InformacionTermino>::const_iterator i = indice.find(w);
    if (i != indice.end()) {
        inf = i->second;
        return true;
    } else if (almacenarEnDisco) {
        unordered_map<string, int>::const_iterator ind = auxDisco.find(w);
        if (ind != auxDisco.end()) {
            string str = getTermDisco(auxDisco.at(w));
            //inf =
            return true;
        }
    }
    inf.~InformacionTermino();
    return false;
}

bool IndexadorHash::Devuelve(string& word, const string& nomDoc, InfTermDoc& infDoc) {
    stemmerPorter stem;
    string w = word;
    tok.Tokenizar(word);
    stem.stemmer(w, tipoStemmer);

    try {
        InformacionTermino inf = indice.at(w);
        infDoc = inf.getL_docs().at(indiceDocs.at(nomDoc).getIdDoc());
        return true;
    } catch (const out_of_range& e){
        return false;
    }
}

bool IndexadorHash::Existe(string& word) {
    stemmerPorter stem;
    string w = word;
    tok.Tokenizar(w);
    stem.stemmer(w, tipoStemmer);

    unordered_map<string, InformacionTermino>::const_iterator i = indice.find(w);
    if (i != indice.end()) {
        return true;
    } else if (almacenarEnDisco) {
        unordered_map<string, int>::const_iterator ind = auxDisco.find(w);
        if (ind != auxDisco.end()) {
            return true;
        }
    }
    return false;
}

bool IndexadorHash::Borra(string& word) {
    stemmerPorter stem;
    string w = word;
    tok.Tokenizar(word);
    stem.stemmer(w, tipoStemmer);

    unordered_map<string, InformacionTermino>::const_iterator i = indice.find(w);
    if(i != indice.end()) {
        indice.erase(w);
        return true;
    } else if (almacenarEnDisco) {
        unordered_map<string, int>::const_iterator ind = auxDisco.find(w);
        if(ind != auxDisco.end()) {
            int numLine = ind->second;
            fstream infile;
            infile.open(nombreDirectorio, ios::in);
            if(!infile) {
                cerr << "ERROR: " << nombreDirectorio << " fichero no encontrado.\n";
                return false;
            }
            int count = 0;
            fstream tempfile;
            tempfile.open("tempfile.txt", ios::out);
            while(!infile.eof()) {
                if (count != numLine) {
                    string line;
                    infile >> line;
                    tempfile << line << '\n';
                }
                ++count;
            }
            infile.close();
            tempfile.close();
            // delete the original file
            remove(nombreDirectorio);
            // rename old to new
            rename("tempfile.txt",nombreDirectorio);
            auxDisco.erase(w);
            return true;
        }
    }
    return false;
}

bool IndexadorHash::BorraDoc(string& nomDoc) {
    try {
        InfDoc doc = indiceDocs.at(nomDoc);
        int id = doc.getIdDoc();
        unordered_set<string> idDocBorr;
        for (auto &elem : indice) {
            const auto& docs = elem.second.getL_docs();
            if (docs.find(id) != docs.end()) {
                elem.second.setFtc(elem.second.getFtc() - docs.at(id).getFt());
                const_cast<unordered_map<int, InfTermDoc>&>(docs).erase(id);
            }
            if(elem.second.getFtc() == 0) {
                idDocBorr.insert(elem.first);
            }
        }
        for (const auto &i : idDocBorr) {
            indice.erase(i);
        }
        indiceDocs.erase(nomDoc);
        ObtenerInfo();
    } catch (const out_of_range& e) {
        return false;
    }
    return true;
}

void IndexadorHash::RestablecerDocsIndice() {
    indiceDocs.clear();
    indice.clear();
}

void IndexadorHash::RestablecerPregIndice() {
    indicePregunta.clear();
}

bool IndexadorHash::Actualiza(const string& word, const InformacionTermino& inf) {
    try {
        indice.at(word) = inf;
        return true;
    } catch (const out_of_range& e) {
        return false;
    }
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
    string linea = "";
    fstream i;
    i.open(ficheroStopWords,ios::in);
    if (i) {
        while(!i.eof()){
            string linea;
            i >> linea;
            if(linea != "") {
                cout << linea << '\n';
            }
        }
        i.close();
    }else{
        cerr << "Error " << ficheroStopWords << " txt no existe.\n";
        i.close();
    }
}

int IndexadorHash::NumPalParada() const {
    return stopWords.size();
}

std::string IndexadorHash::DevolverDelimitadores() const {
    return tok.DelimitadoresPalabra();
}

bool IndexadorHash::DevolverCasosEspeciales() const {
    return tok.CasosEspeciales();
}

bool IndexadorHash::DevolverPasarAminuscSinAcentos() const {
    return tok.PasarAminuscSinAcentos();
}

std::string IndexadorHash::DevolverDirIndice() const {
    return directorioIndice;
}

int IndexadorHash::DevolverTipoStemming() const {
    return tipoStemmer;
}

bool IndexadorHash::DevolverAlmacenarPosTerm() const {
    return almacenarPosTerm;
}

bool IndexadorHash::DevolverAlmEnDisco() const {
    return almacenarEnDisco;
}

void IndexadorHash::ListarInfColeccDocs() const {
    cout << informacionColeccionDocs << '\n';
}

void IndexadorHash::ListarTerminos() const {
    for (const auto& termino : indice){
        cout << termino.first << '\t' << termino.second << '\n';
    }
}

bool IndexadorHash::ListarTerminos(const string& nomDoc) const {
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

void IndexadorHash::ListarDocs() const {
    for(const auto& doc : indiceDocs){
        cout << doc.first << '\t' << doc.second << '\n';
    }
}

bool IndexadorHash::ListarDocs(const string &nomDoc) const {
    try {
        InfDoc doc = indiceDocs.at(nomDoc);
        cout << nomDoc << '\t' << doc << '\n';
        return true;
    } catch (const out_of_range& e){
        return false;
    }
}

string IndexadorHash::getTermDisco(int line) const {
    string str= "";
    fstream infile;
    infile.open(nombreDirectorio, ios::in);
    if(!infile) {
        cerr << "ERROR: " << nombreDirectorio << " fichero no encontrado.\n";
    } else {
        infile.seekg(ios::beg);
        for (int i = 0; i < line; ++i) {
            infile.ignore(numeric_limits<std::streamsize>::max(), '\n');
        }
        getline(infile, str);
        infile.close();
    }
    return str;
}

void
IndexadorHash::IndexarDocumento(const string& name, const int& id, const list<string> tokens) {
    stemmerPorter stem;
    int numPalDif = 0, numPalSinPar = 0, numPal = 0;
    cout << tokens.size() <<" numero de tokens" <<endl;


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
                    termDoc.nuevaPosicionTermino(numPal);
                }
                term.nuevoL_docs(id, termDoc);
                try {
                    indice.insert(pair<string, InformacionTermino>(t, term));
                } catch (const bad_alloc &e) {
                    cerr << "ERROR: Falta de memoria principal. ";
                    if (!almacenarEnDisco) {
                        cerr << "Ultimo documento indexado: " << indiceDocs.end()->first
                             << " Ultimo termino indexado: " << indice.end()->first << "\n";
                        return;
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
                        termDoc.nuevaPosicionTermino(numPal);
                    }
                } else {
                    ++numPalDif;
                    termDoc.AumentoFt();
                    if (almacenarPosTerm) {
                        termDoc.nuevaPosicionTermino(numPal);
                    }
                    term.nuevoL_docs(id, termDoc);
                }
            }
        }
        ++numPal;
    }
    cout << numPal <<" numero de palabras" <<endl;
    struct stat buf;
    stat(name.c_str(), &buf);

    indiceDocs.insert(pair<string, InfDoc> (name, InfDoc(time(0),numPalDif, (int) *&buf.st_size, numPalSinPar, id, numPal)));



}

int IndexadorHash::countFileLines(const string& filename) {
    int count = -1;    // Variable to keep count of each line
    fstream file;
    file.open(filename, ios::in);
    if(!file) {
        cerr << "ERROR: " << filename << " fichero no encontrado.\n";
    } else {
        while(!file.eof()) {
            count++;
        }
        file.close();
    }
    return count;
}

void IndexadorHash::AlmacenarEnDisco(const string& token, const InformacionTermino& termino) {
    string line;
    /**
     * ftc:idDoc/ft/posTerm posTerm posTerm ...:idDoc/ft/posTerm posTerm posTerm...
     */
    int numLines = countFileLines(nombreDirectorio);
    if (auxDisco.find(token) == auxDisco.end()) {
        line = to_string(termino.getFtc()) + ':';
        for (const auto& elem : termino.getL_docs()) {
            line += to_string(elem.first) + '/' +
                    to_string(elem.second.getFt()) + '/';
            if (almacenarPosTerm) {
                for (const auto& pos : elem.second.getPosTerm()) {
                    line += to_string(pos) + ' ';
                }
            }
            line += ':';
        }
        line.erase(prev(line.end())); // remove last ':'
        fstream outfile;
        outfile.open(nombreDirectorio, ios::app);
        if (!outfile) {
            cerr << "ERROR: fichero no se abre.\n";
            return;
        } else {
            auxDisco.insert(pair<string, int>(token, numLines + 1));
            outfile << line << '\n';
        }
        outfile.close();
    } else {
        fstream outfile, tempfile;
        outfile.open(nombreDirectorio, ios::out);
        tempfile.open("tempfile.txt", ios::in);
        if (!outfile || !tempfile) {
            cerr << "ERROR: fichero no se abre.\n";
            return;
        } else {
            int myline = auxDisco.at(token);
            int isline = 0;
            while (getline(outfile, line)) {
                if (myline == isline) {
                    tempfile << to_string(termino.getFtc()) << ':';
                    string aux = "";
                    for (const auto& elem : termino.getL_docs()) {
                        aux += to_string(elem.first) + '/' +
                               to_string(elem.second.getFt()) + '/';
                        if (almacenarPosTerm) {
                            for (const auto& pos : elem.second.getPosTerm()) {
                                aux += to_string(pos) + ' ';
                            }
                        }
                        aux += ':';
                    }
                    aux.erase(prev(line.end()));
                    tempfile << aux << '\n';
                } else {
                    tempfile << line << '\n';
                }
                ++isline;
            }
        }
        outfile.close();
        tempfile.close();
        // delete the original file
        remove(nombreDirectorio);
        // rename old to new
        rename("tempfile.txt",nombreDirectorio);
    }
}

unordered_map<string, InfDoc> IndexadorHash::getIndiceDocs() const {
    return indiceDocs;
}

InfColeccionDocs IndexadorHash::getInfColDocs() const {
    return informacionColeccionDocs;
}

unordered_map<string, InformacionTerminoPregunta> IndexadorHash::getIndicePreg() const {
    return indicePregunta;
}

unordered_map<string, InformacionTermino> IndexadorHash::getIndice() const {
    return indice;
}

string IndexadorHash::getPregunta() const {
    return pregunta;
}
/*
bool
IndexadorHash::Indexar(const string& docCarpeta){
    fstream i;
    list<string> elementos;

    i.open(docCarpeta, ios::in);
    if(i) {
        cerr << "Error " << docCarpeta << " carpeta no existe.\n";
        return false;
    }

    while(!i.eof()) {
        string doc, line;
        i >> doc;
        if (doc == "") {
            return false;
        }
        else{
            fstream i2;
            i2.open(doc, ios::in);
            if(!i2) {
                cerr << "Error " << doc << " carpeta no existe.\n";
                return false;
            }

            elementos.clear();
            while (getline(i2, line)) {
                if (line != "") {
                    tok.Tokenizar(line, elementos);
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

                    for (const auto& token : elementos) {
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
                                    cerr << "Error, falta de memoria";
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
                                        termDoc.nuevaPosicionTermino(numPal);
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
            i2.close();
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

void IndexadorHash::RestablecerDocsIndice() {
    indiceDocs.clear();
    indice.clear();
}

void IndexadorHash::RestablecerPregIndice() {
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
 */

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