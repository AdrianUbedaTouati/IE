#include <vector>
#include "../include/indexadorHash.h"
#include "../include/stemmer.h"

IndexadorHash::IndexadorHash (const string& fichStopWords, const string& delimitadores,
                              const bool& detectComp, const bool& minuscSinAcentos, const string& dirIndice,
                              const int& tStemmer, const bool& almEnDisco, const bool& almPosTerm){
    tok = Tokenizador(delimitadores,detectComp,minuscSinAcentos);
    almacenarEnDisco = almEnDisco;
    almacenarPosTerm = almPosTerm;
    ficheroStopWords = fichStopWords;

    if (dirIndice != "") {
        directorioIndice = dirIndice;
    } else {
        directorioIndice = "./";
    }

    if (directorioIndice.empty())
    {
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

IndexadorHash::IndexadorHash (const IndexadorHash& indx)
{
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

IndexadorHash::IndexadorHash ()
{
    informacionColeccionDocs.~InfColeccionDocs();
    infPregunta.~InformacionPregunta();

    stopWords.clear();

    tok = Tokenizador();

    almacenarEnDisco = false;
    almacenarPosTerm = false;

    ficheroStopWords = "";
    directorioIndice = "./";

    tipoStemmer = 0;
}

IndexadorHash::~IndexadorHash ()
{
    informacionColeccionDocs.~InfColeccionDocs();
    infPregunta.~InformacionPregunta();

    almacenarPosTerm = false;
    almacenarEnDisco = false;

    stopWords.clear();

    ficheroStopWords ="";
    directorioIndice = "";
    pregunta ="";

    tipoStemmer = 0;
}

IndexadorHash&
IndexadorHash::operator= (const IndexadorHash& indx)
{
    if (this == &indx) {
        return *this;
    }else{
        this->~IndexadorHash();
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
        return *this;
    }
}

IndexadorHash::IndexadorHash(const string& directorioIndexacion) {
    directorioIndice = directorioIndexacion;
}

string
IndexadorHash::aplicarStemming(const string& pal) const
{
    string aux = pal;

    if (DevolverPasarAminuscSinAcentos())
    {
        tok.PasarAminuscSinAcentosFun(aux);
    }
    stemmerPorter stemmer;
    stemmer.stemmer(aux, tipoStemmer);

    return aux;
}

void
IndexadorHash::ficheroTratar (const string& nombre, const unordered_map<string, InfDoc>::iterator& mapaDoc)
{
    int auxNumPalSinParada = 0;
    int auxNumPalDiferentes = 0;
    int auxNuevasPal = 0;
    int posicionPal = 0;
    string line = "";

    pair<unordered_map<string, InformacionTermino>::iterator, bool> anadirPal;

    auto listaTokens= recogerTokens(nombre, mapaDoc);

    for(auto elem : listaTokens)
    {
        line = aplicarStemming(elem);

        if (stopWords.find(line) == stopWords.cend())
        {
            auxNumPalSinParada++;
            anadirPal = indice.insert(pair<string, InformacionTermino>(line, InformacionTermino()));
            if (anadirPal.second)
            {
                auxNuevasPal++;
                auxNumPalDiferentes++;
            }
            else if (!anadirPal.first->second.contieneEnDoc(mapaDoc->second.getIdDoc()))
            {
                auxNumPalDiferentes++;

            }
            anadirPal.first->second.incrementarFrecuencia(mapaDoc->second.getIdDoc(), posicionPal);
        }
        if (almacenarPosTerm)
        {
            posicionPal++;
        }
    }

    informacionColeccionDocs.aumentoTamBytes(mapaDoc->second.getTamBytes());
    informacionColeccionDocs.aumentoNumTotalPalSinParada(auxNumPalSinParada);
    informacionColeccionDocs.aumentoNumTotalPal(listaTokens.size());
    informacionColeccionDocs.aumentoNumTotalPalDiferentes(auxNuevasPal);

    mapaDoc->second.aumentoNumPalDiferentes(auxNumPalDiferentes);
    mapaDoc->second.aumentoNumPalSinParada(auxNumPalSinParada);
}



list<string>
IndexadorHash::recogerTokens (const string& nombre, const unordered_map<string, InfDoc>::iterator& mapaDoc) {
    struct stat buf;
    stat(nombre.c_str(), &buf);

    mapaDoc->second.setTamBytes(*&buf.st_size);

    string line = "";

    ifstream docToken;

    docToken.open("tokens.tk");

    list<string> listaTokens;
    if (docToken) {
        while (!docToken.eof()) {
            getline(docToken, line);
            if (line != "") {
                listaTokens.push_back(line);
            }
        }
        docToken.close();
    }

    mapaDoc->second.setNumPal(listaTokens.size());
    return listaTokens;
}


bool
IndexadorHash::Indexar (const string& nombreDoc)
{
    bool info = false;
    list<string> DocsAIndexar;

    struct stat detallesFichero;
    string line = "";

    int numDocs = 0;
    pair<unordered_map<string, InfDoc>::iterator, bool> meterDocumento;

    ifstream f;

    f.open(nombreDoc.c_str());

    if (f) {
        while (!f.eof()) {
            getline(f, line);
            if (line != "") {
                if (stat(line.c_str(), &detallesFichero) != 0) {
                    cout << "ERROR: No se ha encontrado el fichero " << line << endl;
                } else {
                    DocsAIndexar.push_back(line);
                }

            }
        }

        f.close();
        info = true;
    }
    if(info){
        try
        {
            for (string &elem : DocsAIndexar)
            {
                meterDocumento = indiceDocs.insert(pair<string, InfDoc>(elem, InfDoc()));
                if (meterDocumento.second)
                {
                    numDocs++;
                    meterDocumento.first->second.setIdDoc(id);
                    tok.Tokenizar(elem, "tokens.tk");
                    ficheroTratar(elem, meterDocumento.first);

                    id++;
                }
                else
                {
                    struct stat datosDoc;
                    stat(elem.c_str(), &datosDoc);
                    if (datosDoc.st_mtime- meterDocumento.first->second.getFechaModificacion() > 0 ||datosDoc.st_mtime- meterDocumento.first->second.getFechaModificacion() < 0 )
                    {
                        int id = meterDocumento.first->second.getIdDoc();

                        BorraDoc(elem);

                        unordered_map<string, InfDoc>::iterator posicion = indiceDocs.insert(pair<string, InfDoc>(elem, InfDoc())).first;

                        posicion->second.setIdDoc(id);
                        posicion->second.setFechaModificacion(datosDoc.st_mtime);
                        posicion->second.setTamBytes(datosDoc.st_size);

                        ficheroTratar(elem, meterDocumento.first);
                    }
                }
            }
            informacionColeccionDocs.aumentoNumDocs(numDocs);
            return true;
        }
        catch(exception error)
        {
            cerr << error.what() << '\n';
            return false;
        }
    }else{
        cout << "ERROR: En Indexar con el documento " << nombreDoc << endl;
        return false;
    }
    return false;
}

bool
IndexadorHash::IndexarDirectorio (const string& carpetaIndexar)
{
    bool resultado = false;

    auto carpeta = opendir(carpetaIndexar.c_str());

    if (!carpeta)
    {
        return resultado;
    }
    else{
        string orden = "find " + carpetaIndexar + " -type f | sort > carpeta.txt";
        system(orden.c_str());
        resultado = Indexar("carpeta.txt");

        closedir(carpeta);

        return resultado;
    }
}

//TODO
bool
IndexadorHash::GuardarIndexacion () const
{
    DIR * carpeta = opendir(directorioIndice.c_str());

    if (carpeta){
        ofstream ficheroIndexacion("indexado.txt");
    }

    if (!carpeta)
    {
        string comando = "mkdir " + directorioIndice;
        system(comando.c_str());
        carpeta = opendir(directorioIndice.c_str());

        if (carpeta){
            ofstream ficheroIndexacion("indexado.txt");
        }
    }

    return false;
}

void
IndexadorHash::ImprimirIndexacion () const
{
    cout<< "Terminos indexados: \n";
    for (auto &elem :indice)
    {
        cout << " " << elem.first << "\t" << elem.second << "\n";
    }

    cout << "Documentos indexados: ";
    for (auto &elem : indiceDocs)
    {
        cout << elem.first + "\t" << elem.second << "\n";
    }
}

bool
IndexadorHash::IndexarPregunta (const string& cuestion)
{
    indicePregunta.clear();
    infPregunta.~InformacionPregunta();

    if (cuestion == "")
    {
        cout << "ERROR: No se ha podido indexar la pregunta al estar vacía\n";
        return false;
    }else{
        try
        {
            list<string> tokensLista;
            tok.Tokenizar(cuestion, tokensLista);
            infPregunta.aumentoNumTotalPal(tokensLista.size());

            int auxNumPalSinParada = 0;
            int auxNuevasPal = 0;
            int posicion = 0;

            pregunta = cuestion;

            for (string &elem : tokensLista)
            {
                if (stopWords.cend() ==  stopWords.find(elem))
                {
                    auxNumPalSinParada++;
                    auto insercionPal = indicePregunta.insert(pair<string, InformacionTerminoPregunta>(elem, InformacionTerminoPregunta()));
                    if (insercionPal.second)
                    {
                        auxNuevasPal++;
                    }

                    insercionPal.first->second.incrementarFrecuencia(posicion);
                }
                if (almacenarPosTerm)
                {
                    posicion++;
                }
            }
            infPregunta.aumentoNumTotalPalDiferentes(auxNuevasPal);
            infPregunta.aumentoNumTotalPalSinParada(auxNumPalSinParada);

            return true;
        }
        catch(exception& error)
        {
            cerr << error.what() << '\n';
            return false;
        }
    }
}

bool
IndexadorHash::DevuelvePregunta (string& cuestion) const
{
    if (indicePregunta.size() == 0) {
      return false;
    }else{
        cuestion = pregunta;
        return true;
    }
}

bool
IndexadorHash::DevuelvePregunta (const string& palabra, InformacionTerminoPregunta& aux) const
{
    auto pos = indicePregunta.find(aplicarStemming(palabra));

    if (indicePregunta.cend() == pos)
    {
        return false;
    }else{
        aux = pos->second;
        return true;
    }
}

bool
IndexadorHash::DevuelvePregunta (InformacionPregunta& aux) const
{
    if (indicePregunta.size() == 0) {
        return false;
    }else{
        aux = infPregunta;
        return true;
    }
}

void
IndexadorHash::ImprimirIndexacionPregunta ()
{
    cout << "Pregunta indexada: " << pregunta << "\n" << "Terminos indexados en la pregunta: \n";

    for (auto& elem : indicePregunta)
    {
        cout << elem.first << "\t" << elem.second << "\n";
    }

    cout << infPregunta;
}

void
IndexadorHash::ImprimirPregunta ()
{
    cout << "Pregunta indexada: " << pregunta << "\n"<<"Informacion de la pregunta: " << infPregunta;
}

bool
IndexadorHash::Devuelve (const string& palabra, InformacionTermino& aux) const
{
    if (!Existe(palabra))
    {
      return false;
    }else{
        aux = indice.at(aplicarStemming(palabra));
        return true;
    }
}

bool
IndexadorHash::Devuelve (const string& palabra, const string& documentoNombre, InfTermDoc& aux) const
{
    if(Existe(palabra)){
        auto documento = indiceDocs.find(documentoNombre);
        if(documento == indiceDocs.cend()){
            aux.~InfTermDoc();
            return false;
        }else{
            auto auxTermino = indice.find(aplicarStemming(palabra));
            aux = auxTermino->second.getInfTermDoc(documento->second.getIdDoc());
            return true;
        }
    }
    else{
        aux.~InfTermDoc();
        return false;
    }
}

bool
IndexadorHash::Existe (const string& palabra) const
{
    if(indice.find(aplicarStemming(palabra)) != indice.end()){
        return true;
    }else{
        return false;
    }
}

bool
IndexadorHash::Borra (const string& palabra)
{
    if (!Existe(palabra))
    {
        return false;
    }else{
        indice.erase(aplicarStemming(palabra));
        return true;
    }
}


bool
IndexadorHash::BorraDoc (const string& ficheroNombre)
{
    auto auxPos = indiceDocs.find(ficheroNombre);

    if (auxPos == indiceDocs.end())
    {
        return false;
    }else{
        int id = auxPos->second.getIdDoc();

        string eliminar = "";

        informacionColeccionDocs.aumentoNumDocs(-1);
        informacionColeccionDocs.aumentoNumTotalPal(- auxPos->second.getNumPal());
        informacionColeccionDocs.aumentoTamBytes(- auxPos->second.getTamBytes());
        informacionColeccionDocs.aumentoNumTotalPalSinParada(- auxPos->second.getNumPalSinParada());

        unordered_map<int, InfTermDoc>::const_iterator indiceAuxPos;

        for (auto &pos : indice)
        {
            if (eliminar.size() != 0)
            {
                indice.erase(eliminar);
            }

            eliminar = "";

            if (pos.second.contieneEnDoc(id))
            {
                pos.second.suprimirDocumento(id);
                if (pos.second.getL_docs().size() == 0)
                {
                    eliminar = pos.first;
                }
            }
        }
        if (eliminar.size() != 0)
        {
            indice.erase(eliminar);
        }
        indiceDocs.erase(ficheroNombre);
        informacionColeccionDocs.setNumTotalPalDiferentes(indice.size());
        return true;
    }
}

void
IndexadorHash::VaciarIndiceDocs ()
{
    indiceDocs.clear();
}

void
IndexadorHash::VaciarIndicePreg ()
{
    indicePregunta.clear();
}

bool
IndexadorHash::Actualiza (const string& palabra, const InformacionTermino& aux)
{
    if (!Existe(palabra))
    {
        return false;
    }else{
        indice.at(aplicarStemming(palabra)) = aux;
        return true;
    }
}

bool
IndexadorHash::Inserta (const string& palabra, const InformacionTermino& aux)
{
    if (!Existe(palabra)) {
        return false;
    }else{
        pair<string, InformacionTermino> nuevoTermino(aplicarStemming(palabra), aux);
        indice.insert(nuevoTermino);
        return true;
    }
}

int
IndexadorHash::NumPalIndexadas() const
{
    return indice.size();
}

string
IndexadorHash::DevolverFichPalParada() const
{
    return ficheroStopWords;
}

void
IndexadorHash::ListarPalParada() const
{
    for (auto& elem : stopWords)
    {
        cout << elem << "\n";
    }
}

int
IndexadorHash::NumPalParada() const
{
    return stopWords.size();
}

string
IndexadorHash::DevolverDelimitadores() const
{
    return tok.DelimitadoresPalabra();
}

bool
IndexadorHash::DevolverCasosEspeciales() const
{
    return tok.CasosEspeciales();
}

bool
IndexadorHash::DevolverPasarAminuscSinAcentos() const
{
    tok.PasarAminuscSinAcentos();
}

bool
IndexadorHash::DevolverAlmacenarPosTerm() const
{
    return almacenarPosTerm;
}

string
IndexadorHash::DevolverDirIndice() const
{
    return directorioIndice;
}

int
IndexadorHash::DevolverTipoStemming() const
{
    return tipoStemmer;
}

bool
IndexadorHash::DevolverAlmEnDisco() const
{
    return almacenarEnDisco;
}

void
IndexadorHash::ListarInfColeccDocs() const
{
    cout << informacionColeccionDocs.ToString() << endl;
}

void
IndexadorHash::ListarTerminos() const
{
    for (auto &elem : indice)
    {
        cout << elem.first << "\t" << elem.second << "\n";
    }
}

bool
IndexadorHash::ListarTerminos (const string& ficheroNombre) const
{
    auto doc = indiceDocs.find(ficheroNombre);

    if (doc == indiceDocs.cend()) {
        return false;
    }else{
        int id = doc->second.getIdDoc();

        for (auto &elem : indice)
        {
            if (elem.second.contieneEnDoc(id))
            {
                cout << elem.first << elem.second << "\n";
            }
        }
        return true;
    }
}

void
IndexadorHash::ListarDocs () const
{
    for (auto &elem : indiceDocs)
    {
        cout << elem.first << "\t" << elem.second << "\n";
    }
}

bool
IndexadorHash::ListarDocs (const string& ficheroNombre) const
{
    auto doc = indiceDocs.find(ficheroNombre);

    if (doc == indiceDocs.cend())
    {
        return false;
    }else{
        cout << ficheroNombre << "\t" << doc->second << "\n";
        return true;
    }
}