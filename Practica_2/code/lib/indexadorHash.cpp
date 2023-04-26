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

/*

// Constructor de copia
IndexadorHash::IndexadorHash (const IndexadorHash& indexador)
{
    indice = indexador.indice;
    indiceDocs = indexador.indiceDocs;
    informacionColeccionDocs = indexador.informacionColeccionDocs;
    pregunta = indexador.pregunta;
    indicePregunta = indexador.indicePregunta;
    infPregunta = indexador.infPregunta;
    stopWords = indexador.stopWords;
    ficheroStopWords = indexador.ficheroStopWords;
    tok = indexador.tok;
    directorioIndice = indexador.directorioIndice;
    tipoStemmer = indexador.tipoStemmer;
    almacenarEnDisco = indexador.almacenarEnDisco;
    almacenarPosTerm = indexador.almacenarPosTerm;
}

// Constructor por defecto
IndexadorHash::IndexadorHash ()
{
    pregunta = "";
    directorioIndice = "";
    tipoStemmer = 0;
    almacenarEnDisco = false;
    almacenarPosTerm = false;
}

// Destructor
IndexadorHash::~IndexadorHash ()
{
    indice.clear();
    indiceDocs.clear();
    informacionColeccionDocs.~InfColeccionDocs();
    pregunta.clear();
    indicePregunta.clear();
    infPregunta.~InformacionPregunta();
    stopWords.clear();
    ficheroStopWords.clear();
    //tok.~Tokenizador();
    tipoStemmer = 0;
    almacenarEnDisco = false;
    almacenarPosTerm = false;
}

// Operador asignación
IndexadorHash&
IndexadorHash::operator= (const IndexadorHash& indexador)
{
    if (this != &indexador)
    {
        this->~IndexadorHash();
        // Se copia
        indice = indexador.indice;
        indiceDocs = indexador.indiceDocs;
        informacionColeccionDocs = indexador.informacionColeccionDocs;
        pregunta = indexador.pregunta;
        indicePregunta = indexador.indicePregunta;
        infPregunta = indexador.infPregunta;
        stopWords = indexador.stopWords;
        ficheroStopWords = indexador.ficheroStopWords;
        tok = indexador.tok;
        directorioIndice = indexador.directorioIndice;
        tipoStemmer = indexador.tipoStemmer;
        almacenarEnDisco = indexador.almacenarEnDisco;
        almacenarPosTerm = indexador.almacenarPosTerm;
    }

    return *this;
}

IndexadorHash::IndexadorHash (const string& fichStopWords, const string& delimitadores,
                              const bool& detectComp, const bool& minuscSinAcentos, const string& dirIndice,
                              const int& tStemmer, const bool& almEnDisco, const bool& almPosTerm)
{
    ficheroStopWords = fichStopWords;
    // Almacena todas las stopwords del fichero
    almacenarStopWords();
    // Se inicializa el tokenizador
    tok.DelimitadoresPalabra(delimitadores);
    tok.CasosEspeciales(detectComp);
    tok.PasarAminuscSinAcentos(minuscSinAcentos);
    directorioIndice = dirIndice;
    if (directorioIndice.empty())
    { // Si está vacío
        // Se almacenará en el directorio donde se ejecuta el proyecto
        directorioIndice = "./";
    }
    tipoStemmer = tStemmer;
    almacenarEnDisco = almEnDisco;
    almacenarPosTerm = almPosTerm;
}

bool
IndexadorHash::almacenarStopWords ()
{
    ifstream fLectura;
    string linea = ""; // En cada línea habrá una stopWord

    fLectura.open(ficheroStopWords.c_str());

    if (fLectura.is_open())
    { // Si se abre el fichero del que se quiere leer
        while (!fLectura.eof())
        { // Mientras no se llegue al final del fichero
            linea = "";
            // Se lee la línea (que contiene una palabra)
            getline(fLectura, linea);
            if (!linea.empty())
            { // Si la línea no está vacía
                // Se inserta a stopWords
                stopWords.insert(linea);
            }
        }
        fLectura.close();
        return true;
    }

    cerr << "ERROR: No existe el archivo: -" << ficheroStopWords << "-" << endl;
    return false;
}
*/


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

/*
void
IndexadorHash::reindexarDocumento (const string& nombreDoc, const unordered_map<string, InfDoc>::iterator& itDoc, const off_t& tamB, const time_t& fMod)
{
    long int auxId = itDoc->second.getIdDoc();
    // Se elimina toda la información del índice
    BorraDoc(nombreDoc);
    // Se vuelve a insertar el documento
    unordered_map<string, InfDoc>::iterator posicion = indiceDocs.insert(pair<string, InfDoc>(nombreDoc,InfDoc())).first;
    // Se establece la antigua id del documento
    posicion->second.setIdDoc(auxId);
    // Se establece la fecha de modificación
    posicion->second.setFechaModificacion(fMod);
    // Se establece el tamaño del documento
    posicion->second.setTamBytes(tamB);
    // Se indexa
    indexarDocumento(nombreDoc, itDoc);
}


// Devuelve TRUE si recuperan los documentos correctamente
bool
IndexadorHash::cargarDocsAindexar (const string& ficheroDocumentos, list<string>& listaDocs) const
{
    ifstream f;
    struct stat infoFichcero;
    f.open(ficheroDocumentos.c_str());

    if (f.is_open())
    {
        string linea;
        while (!f.eof())
        {
            getline(f, linea);
            if (!linea.empty())
            {
                if (stat(linea.c_str(), &infoFichcero) == 0)
                { // Si el fichero existe
                    listaDocs.push_back(linea);
                }
                else
                {
                    cout << "ERROR: No se ha encontrado el fichero " << linea << endl;
                }

            }
        }

        f.close();

        return true;
    }

    return false;
}

// Devuelve TRUE si se crea el índice para la colección de documentos de "ficheroDocumentos".
bool
IndexadorHash::Indexar (const string& ficheroDocumentos)
{
    list<string> listaDocsIndexar;

    if (cargarDocsAindexar(ficheroDocumentos, listaDocsIndexar))
    { // Si se han cargado los ficheros correctamente
        // Se indexa cada uno de ellos
        try
        {
            long int auxNumDocs = 0;
            pair<unordered_map<string, InfDoc>::iterator, bool> insercionDoc;
            for (string doc : listaDocsIndexar)
            { // Para cada documento
                // Se intenta insertar
                insercionDoc = indiceDocs.insert(pair<string, InfDoc>(doc, InfDoc()));
                if (insercionDoc.second)
                { // Si el documento ha sido insertado (no había sido indexado)
                    // Se le asigna una id
                    insercionDoc.first->second.setIdDoc(id);
                    // Se incrementa el número de documentos en la indexación
                    auxNumDocs++;
                    // Se tokeniza
                    tok.Tokenizar(doc, "tokens.tk");
                    // Se indexa el documento
                    indexarDocumento(doc, insercionDoc.first);
                    // Se incrementa la id del documento, para el siguiente
                    id++;
                }
                else
                { // Si ya había sido indexado
                    // Se extrae la información del fichero
                    struct stat infoFichero;
                    stat(doc.c_str(), &infoFichero);
                    if (difftime(infoFichero.st_mtime, insercionDoc.first->second.getFechaModificacion()) > 0)
                    { // Si la fecha de modificación es más reciente (mayor)
                        // Se reindexa
                        reindexarDocumento(doc, insercionDoc.first, infoFichero.st_size, infoFichero.st_mtime);
                    }
                }
            }
            informacionColeccionDocs.aumentoNumDocs(auxNumDocs);
            return true;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return false;
        }
    }
    else
    {
        cout << "ERROR: En Indexar con el documento " << ficheroDocumentos << endl;
    }

    return false;
}


// Indexa el documento que se pasa por parámetro
void
IndexadorHash::ficheroTratar (const string& nombreDoc, const unordered_map<string, InfDoc>::iterator& itDoc)
{
    // Se recupera la tokenización, para indexar las palabras del documento
    ifstream token;
    token.open("tokens.tk");

    // Se recupera la tokenización
    string linea;
    list<string> tokenizacion;
    if (token.is_open())
    {
        while (!token.eof())
        {
            getline(token, linea);
            if (!linea.empty())
            {
                tokenizacion.push_back(linea);
            }
        }
        token.close();
    }

    struct stat buf;
    stat(nombreDoc.c_str(), &buf);

    // Se almacena el tamaño en bytes del documento
    itDoc->second.setTamBytes(*&buf.st_size);
    // Se almacena la cantidad de palabras del documento
    itDoc->second.setNumPal(tokenizacion.size());
    // Se indexa cada palabra
    string palabra;
    int posicionPal = -1;
    // Número de palabras sin stopwords en el documento
    int auxNumPalSinParada = 0;
    // Número de palabras diferentes en el documento
    int auxNumPalDiferentes = 0;
    // Número de nuevas palabras indexadas
    int auxNuevasPal = 0;
    // Posición de la palabra en el ínidice
    pair<unordered_map<string, InformacionTermino>::iterator, bool> insercionPal;

    for (list<string>::const_iterator pos = tokenizacion.cbegin(); pos != tokenizacion.cend(); pos++)
    {
        if (almacenarPosTerm)
        {
            posicionPal++;
        }
        palabra = aplicarStemming(pos->data());
        if (stopWords.find(palabra) == stopWords.cend())
        { // Si la palabra no es una stopword (si lo es, se ignora)
            // Se incrementa el número de palabras que no son stopwords en el documento
            auxNumPalSinParada++;
            // Se intenta insertar la palabra
            insercionPal = indice.insert(pair<string, InformacionTermino>(palabra,InformacionTermino()));
            if (insercionPal.second)
            { // Si la palabra se ha insertado (es nueva en el índice)
                // Se incrementa el número de nuevas palabras indexadas
                auxNuevasPal++;
                // Se incrementa el número de palabras diferentes en el documento
                auxNumPalDiferentes++;
            }
            else if (!insercionPal.first->second.contieneEnDoc(itDoc->second.getIdDoc()))
            { // Si es nueva en el documento
                // Se incrementa el número de palabras diferentes en el documento
                auxNumPalDiferentes++;
            }
            // Se incrementa la frecuencia del término
            insercionPal.first->second.incrementarFrecuencia(itDoc->second.getIdDoc(), posicionPal);
        }
    }
    itDoc->second.aumentoNumPalDiferentes(auxNumPalDiferentes);
    itDoc->second.aumentoNumPalSinParada(auxNumPalSinParada);

    informacionColeccionDocs.aumentoTamBytes(itDoc->second.getTamBytes());
    informacionColeccionDocs.aumentoNumTotalPalSinParada(auxNumPalSinParada);
    informacionColeccionDocs.aumentoNumTotalPal(tokenizacion.size());
    informacionColeccionDocs.aumentoNumTotalPalDiferentes(auxNuevasPal);
}
*/



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
            for (string elem : DocsAIndexar)
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
                    if (difftime(datosDoc.st_mtime, meterDocumento.first->second.getFechaModificacion()) > 0)
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
        catch(const exception e)
        {
            cerr << e.what() << '\n';
            return false;
        }
    }else{
        cout << "ERROR: En Indexar con el documento " << nombreDoc << endl;
        return false;
    }
    return false;
}



bool
IndexadorHash::IndexarDirectorio (const string& dirAIndexar)
{
  // Puntero para abrir el directorio
  DIR *directorio;
  // Información del fichero que se lee
  struct dirent *infFichero;

  // Se abre el directorio
  directorio = opendir(dirAIndexar.c_str());
  if (directorio)
  { // El directorio existe
    string comando = "find " + dirAIndexar + " -type f | sort > " + "directorio.txt";
    system(comando.c_str());

    closedir(directorio);
    return Indexar("directorio.txt");
  }

  //cout << "ERROR: No se pudo abrir el directorio " << dirAIndexar << endl;
  return false;
}

/* Se guardará en disco duro (incluidos todos los parámetros de la parte privada)
    Devolverá TRUE si finaliza la operación correcctamente
*/
bool
IndexadorHash::GuardarIndexacion () const
{
  // Se crean los punteros para manejar la ruta del directorio
  DIR *directorio;
  struct dirent *entrada;

  // Se intenta abrir el directorio de "directorioIndice"
  directorio = opendir(directorioIndice.c_str());
  if (!directorio)
  { // Si no existe
    // Se crea
    string comando = "mkdir " + directorioIndice;
    system(comando.c_str());
    // Se vuelve a intentar abrir el directorio
    directorio = opendir(directorioIndice.c_str());
  }

  if (directorio)
  { // Si el directorio existe o ha sido creado correctamente
    // Abre o crea el fichero donde se guardará la indexación
    ofstream ficheroIndexacion("indexado.txt");

    //TODO
  }  

  return false;
}

// Lista el contenido del campo "indice" y del campo "indiceDocs"
void
IndexadorHash::ImprimirIndexacion () const
{

  cout<< "Terminos indexados: \n";
  // Lista el contenido de campo "indice"
  for (auto termino = indice.begin(); termino != indice.end(); termino++)
  {
    cout<< " " << termino->first << "\t" << termino->second << "\n";
  }
  cout << "Documentos indexados: ";
  // Lista el contenido del campo indiceDocs
  for (auto doc = indiceDocs.begin(); doc != indiceDocs.end(); doc++)
  {
    cout << doc->first + "\t" << doc->second << "\n";
  }

}

bool
IndexadorHash::IndexarPregunta (const string& preg)
{
  // Se vacían campos
  indicePregunta.clear();
  infPregunta.~InformacionPregunta();

  if (!preg.empty())
  {
    try
    {
      pregunta = preg;
      // Número de palabras que no son stopword en la pregunta
      int auxNumPalSinParada = 0;
      // Número de nuevas palabras indexadas
      int auxNuevasPal = 0;
      // Posición de la palabra en la pregunta
      int posicion = -1;

      // Se tokeniza
      list<string> tokenizacion;
      tok.Tokenizar(preg, tokenizacion);
      // Se almacena la cantidad de palabras de la pregunta
        infPregunta.aumentoNumTotalPal(tokenizacion.size());
      
      // Para cada palabra
      for (string palabra : tokenizacion)
      {
        if (almacenarPosTerm)
        {
          posicion++;
        }
        if (stopWords.find(palabra) == stopWords.cend())
        { // Si no es una stopword
          auxNumPalSinParada++;
          // Se inserta en el índice de la pregunta
          pair<unordered_map<string, InformacionTerminoPregunta>::iterator, bool> insercionPal = indicePregunta.insert(pair<string, InformacionTerminoPregunta>(palabra, InformacionTerminoPregunta()));
          if (insercionPal.second)
          { // Si ha sido insertado (nueva palabra)
            auxNuevasPal++;
          }
          // Se incrementa la frecuencia del término en la pregunta y se almacena la posición en la que aparece la palabra en la pregunta
          insercionPal.first->second.incrementarFrecuencia(posicion);
        }
      }
      // Se incrementa el número de palabras que no son stopwords en la pregunta
        infPregunta.aumentoNumTotalPalSinParada(auxNumPalSinParada);
      // Se actualiza el número de palabras diferentes en la pregunta
        infPregunta.aumentoNumTotalPalDiferentes(auxNuevasPal);

      return true;
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
    }
  }

  cout << "ERROR: No se ha podido indexar la pregunta al estar vacía\n";
  return false;
}

bool
IndexadorHash::DevuelvePregunta (string& preg) const
{
  if (!indicePregunta.empty())
  {
    preg = pregunta;

    return true;
  }

  return false;
}

bool
IndexadorHash::DevuelvePregunta (const string& word, InformacionTerminoPregunta& inf) const
{
  string palabra = aplicarStemming(word);
  unordered_map<string, InformacionTerminoPregunta>::const_iterator posicion = indicePregunta.find(palabra);

  if (posicion != indicePregunta.cend())
  { // Si encuentra la palabra  
    inf = posicion->second;

    return true;
  }

  return false;
}

bool
IndexadorHash::DevuelvePregunta (InformacionPregunta& inf) const
{
  if (!indicePregunta.empty())
  {
    inf = infPregunta;

    return true;
  }

  return false;
}

void
IndexadorHash::ImprimirIndexacionPregunta ()
{
  cout << "Pregunta indexada: " << pregunta << "\nTerminos indexados en la pregunta: \n";
  // Se lista el contenido de "indicePregunta"
  for (unordered_map<string, InformacionTerminoPregunta>::const_iterator pos = indicePregunta.cbegin(); pos != indicePregunta.cend(); pos++)
  {
    cout << pos->first << "\t" << pos->second << "\n";
  }
  cout << infPregunta;
}

void
IndexadorHash::ImprimirPregunta ()
{
  cout << "Pregunta indexada: " << pregunta <<
    "\nInformacion de la pregunta: " << infPregunta;
}

// Devuelve TRUE si word ha sido indexado y devuelve la información en inf
bool
IndexadorHash::Devuelve (const string& word, InformacionTermino& inf) const
{
  string palabra;

  if (Existe(word))
  {
    palabra = aplicarStemming(word);
    inf = indice.at(palabra);
    return true;
  }

  return false;
}

/* Devuelve TRUE si word ha sido indexado y aparece en el documento de nombre nomDoc.
    Devuelve la información almacenada para word en el documento
*/
bool
IndexadorHash::Devuelve (const string& word, const string& nomDoc, InfTermDoc& infDoc) const
{
  unordered_map<string, InfDoc>::const_iterator documento = indiceDocs.find(nomDoc);

  if (Existe(word) && documento != indiceDocs.cend())
  { // Si existe la palabra indexada y el documento
    // Se trata el termino
    string termino = aplicarStemming(word);
    unordered_map<string, InformacionTermino>::const_iterator infoTermino = indice.find(termino);
    // Se recupera la información del término en el documento
    infDoc = infoTermino->second.getInfTermDoc(documento->second.getIdDoc());
    return true;
  }

  // Si no devuelve infDoc vacío
  infDoc.~InfTermDoc();
  return false;
}

bool
IndexadorHash::Existe (const string& word) const
{
  string palabra = word;

    aplicarStemming(palabra);

  return indice.find(palabra) != indice.end();
}

bool
IndexadorHash::Borra (const string& word)
{
  if (Existe(word))
  {
    string palabra = aplicarStemming(word);
    indice.erase(palabra);
    return true;
  }

  return false;
}

bool
IndexadorHash::BorraDoc (const string& nomDoc)
{
  // Se recupera el puntero del nombre del documento
  unordered_map<string, InfDoc>::iterator posicion = indiceDocs.find(nomDoc);

  if (posicion != indiceDocs.end())
  {
    // Se actualizan los datos de la colección (incrementando en negativo)
      informacionColeccionDocs.aumentoNumDocs(-1);
      informacionColeccionDocs.aumentoNumTotalPal(-posicion->second.getNumPal());
      informacionColeccionDocs.aumentoNumTotalPalSinParada(-posicion->second.getNumPalSinParada());
      //informacionColeccionDocs.aumentoNumTotalPalDiferentes(-posicion->second.getNumPalDiferentes());
      informacionColeccionDocs.aumentoTamBytes(-posicion->second.getTamBytes());

    // Se borran todos los términos del documento
    long int id = posicion->second.getIdDoc();
    // Se busca en todos los términos indexados
    unordered_map<long int, InfTermDoc>::const_iterator posIndice;
    // Controla que la posición anterior tenga que ser eliminada
    string eliminar = "";
    for (unordered_map<string, InformacionTermino>::iterator pos = indice.begin(); pos != indice.end(); pos++)
    { // Para cada elemento del índice
      if (!eliminar.empty())
      { // Si hay un término para eliminar
        indice.erase(eliminar);
        eliminar = "";
      }
      // Comprueba que pertenezca al documento
      if (pos->second.contieneEnDoc(id))
      { // Si pertenece al documento
        // Se elimina
          pos->second.suprimirDocumento(id);
        if (pos->second.getL_docs().empty())
        { // Si el término no aparece en más documentos, se debe eliminar
          eliminar = pos->first;
        }
      }
    }
    // Para comprobar si el último también debe ser eliminado
    if (!eliminar.empty())
    { // Si hay un término para eliminar
      indice.erase(eliminar);
    }

    // Después se actualiza la colección de documentos
    informacionColeccionDocs.setNumTotalPalDiferentes(indice.size());
    // Se elimina del índice de documentos
    indiceDocs.erase(nomDoc);

    return true;
  }

  return false;
}

// Borra todos los términos del índice de documentos
void
IndexadorHash::VaciarIndiceDocs ()
{
  indiceDocs.clear();
}

// Borra todos los términos del índice de la pregunta
void
IndexadorHash::VaciarIndicePreg ()
{
  indicePregunta.clear();
}

// Sustituye la información almacenada en el índice por la de "inf"
bool
IndexadorHash::Actualiza (const string& word, const InformacionTermino& inf)
{
  if (Existe(word))
  { // Si existe
    // Actualiza la información del término indexado
    string termino = aplicarStemming(word);
    indice.at(termino) = inf;

    return true;
  }

  return false;
}

/* Se insertará la palabra (habiendo aplicado aplicarStemming y mayúsculas) si no
    estaba previamente indexada
*/
bool
IndexadorHash::Inserta (const string& word, const InformacionTermino& inf)
{
  if (Existe(word))
  { // Si el término no ha sido indexado
    string termino = aplicarStemming(word);
    // Se inserta
    pair<string, InformacionTermino> nuevoTermino(termino, inf);
    indice.insert(nuevoTermino);
    return true;
  }

  return false;
}

// Devuelve el número de términos diferentes indexados
int
IndexadorHash::NumPalIndexadas () const
{
  return indice.size();
}

// Devuelve el contenido del campo "ficheroStopWords"
string
IndexadorHash::DevolverFichPalParada () const
{
  return ficheroStopWords;
}

// Muestra por pantalla las palabras de parada almacenadas
void
IndexadorHash::ListarPalParada () const
{
  string salida;

  for (const string& palParada : stopWords)
  {
    salida += palParada + "\n";
  }

  cout << salida;
}

// Devuelve el número de palabras de parada almacenadas
int
IndexadorHash::NumPalParada () const
{
  return stopWords.size();
}

// Devuelve los delimitadores utilizados por el tokenizador
string
IndexadorHash::DevolverDelimitadores () const
{
  return tok.DelimitadoresPalabra();
}

// Devuelve si el tokenizador analiza los casos especiales
bool
IndexadorHash::DevolverCasosEspeciales () const
{
  return tok.CasosEspeciales();
}

// Devuelve si el tokenizador pasa a minúsculas sin acentos
bool
IndexadorHash::DevolverPasarAminuscSinAcentos () const
{
  tok.PasarAminuscSinAcentos();
}

// Devuelve el valor de almacenarPosTerm
bool
IndexadorHash::DevolverAlmacenarPosTerm () const
{
  return almacenarPosTerm;
}

// Devulve "directorioIndice"
string
IndexadorHash::DevolverDirIndice () const
{
  return directorioIndice;
}

// Devuelve el valor de "tipoStemmer"
int
IndexadorHash::DevolverTipoStemming () const
{
  return tipoStemmer;
}

// Devuekve el valor indicado en "almEnDisco"
bool
IndexadorHash::DevolverAlmEnDisco () const
{
  return almacenarEnDisco;
}

// Muestra por pantalla la información de la colección de documentos
void
IndexadorHash::ListarInfColeccDocs () const
{
  cout << informacionColeccionDocs.ToString() << endl;
}

// Muestra el contenido de "indice"
void
IndexadorHash::ListarTerminos () const
{

  for (unordered_map<string, InformacionTermino>::const_iterator pos = indice.cbegin();
    pos != indice.cend(); pos++)
  {
    cout << pos->first << "\t" << pos->second << "\n";
  }
}

/* Devuelve TRUE si el documento ha sido indexado y muestra por pantalla los términos
    indexados del documento con ese nombre
*/
bool
IndexadorHash::ListarTerminos (const string& nomDoc) const
{
  unordered_map<string, InfDoc>::const_iterator doc = indiceDocs.find(nomDoc);

  if (doc != indiceDocs.cend())
  { // Si el documento ha sido indexado
    // Se buscan los términos indexados procendentes del mismo
    long int id = doc->second.getIdDoc();
    // Recupera todos los términos indexados que pertenezcan a ese documento
    for (unordered_map<string, InformacionTermino>::const_iterator pos = indice.cbegin();
      pos != indice.cend(); pos++)
    {
      if (pos->second.contieneEnDoc(id))
      { // Si el término pertenece al documento, se añade a la cadena de salida
        cout << pos->first << pos->second << "\n";
      }
    }
    // Muestra el término y la información del término

    return true;
  }

  return false;
}

// Muestra el contenido de "indiceDocs"
void
IndexadorHash::ListarDocs () const
{
  for (unordered_map<string, InfDoc>::const_iterator pos = indiceDocs.cbegin();
    pos != indiceDocs.cend(); pos++)
  {
     cout << pos->first << "\t" << pos->second << "\n";
  }
}

// Devuelve TRUE si el documento ha sido indexado, y muestra por pantalla el nombre y la información
bool
IndexadorHash::ListarDocs (const string& nomDoc) const
{
  unordered_map<string, InfDoc>::const_iterator doc = indiceDocs.find(nomDoc);

  if (doc != indiceDocs.cend())
  { // Si el documento ha sido indexado
    // Se muestra su información
    cout << nomDoc << "\t" << doc->second << "\n";

    return true;
  }

  return false;
}