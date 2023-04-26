#ifndef PRACTICA_2_INDEXADORHASH_H
#define PRACTICA_2_INDEXADORHASH_H

#include <iterator>
#include <unordered_set>
#include "tokenizador.h"
#include "indexadorinformacion.h"
#include <dirent.h>

class IndexadorHash
{
friend ostream& operator<<(ostream& s, const IndexadorHash& p) {
    s << "Fichero con el listado de palabras de parada: " << p.ficheroStopWords << '\n';
    s << "Tokenizador: " << p.tok << '\n';
    s << "Directorio donde se almacenara el indice generado: " << p.directorioIndice << '\n';
    s << "Stemmer utilizado: " << p.tipoStemmer << '\n';
    s << "Informacion de la coleccion indexada: " <<p.informacionColeccionDocs.ToString() << '\n';
    s << "Se almacenara parte del indice en disco duro: " <<p.almacenarEnDisco << '\n';
    s << "Se almacenaran las posiciones de los terminos: " <<p.almacenarPosTerm << '\n';
    return s;
}

private:
    IndexadorHash();

    unordered_map<string, InformacionTermino> indice;

    unordered_map<string, InfDoc> indiceDocs;

    InfColeccionDocs informacionColeccionDocs;

    string pregunta;

    unordered_map<string, InformacionTerminoPregunta> indicePregunta;

    InformacionPregunta infPregunta;

    unordered_set<string> stopWords;

    string ficheroStopWords;

    Tokenizador tok;

    string directorioIndice;

    int tipoStemmer;

    bool almacenarEnDisco;

    bool almacenarPosTerm;

    int id = 1;

    //auxiliares metodos

    string aplicarStemming(const string&) const;

    void ficheroTratar (const string&, const unordered_map<string, InfDoc>::iterator&);

    bool obtenerInformacionDocsIndexar (const string&, list<string>&) const;

    bool almacenarStopWords ();

    void soloStemming(string& palabra) const;

    bool cargarDocsAindexar (const string& ficheroDocumentos, list<string>& listaDocs) const;

    void reindexarDocumento (const string& nombreDoc, const unordered_map<string, InfDoc>::iterator& itDoc, const off_t& tamB, const time_t& fMod);

    //void ficheroTratar (const string& nombreDoc, const unordered_map<string, InfDoc>::iterator& itDoc);





    list<string>  recogerTokens (const string& nombre, const unordered_map<string, InfDoc>::iterator& mapaDoc);

public:
    IndexadorHash (const string&, const string&, const bool&, const bool&,
      const string&, const int&, const bool&, const bool&);

    IndexadorHash (const string& directorioIndexacion);



    IndexadorHash (const IndexadorHash&);

    ~IndexadorHash ();

    IndexadorHash& operator= (const IndexadorHash&);

    bool Indexar(const string& nombreDoc);

    bool IndexarDirectorio (const string&);

    bool GuardarIndexacion() const;

    bool RecuperarIndexacion (const string&);

    void ImprimirIndexacion() const;

    bool IndexarPregunta (const string&);

    bool DevuelvePregunta (string&) const;

    bool DevuelvePregunta (const string&, InformacionTerminoPregunta&) const;

    bool DevuelvePregunta (InformacionPregunta&) const;

    void ImprimirIndexacionPregunta ();

    void ImprimirPregunta ();

    bool Devuelve (const string&, InformacionTermino&) const;

    bool Devuelve (const string&, const string&, InfTermDoc&) const;

    bool Existe (const string&) const;

    bool Borra (const string&);

    bool BorraDoc (const string&);

    void VaciarIndiceDocs ();

    void VaciarIndicePreg();

    bool Actualiza (const string&, const InformacionTermino&);

    bool Inserta (const string&, const InformacionTermino&);

    int NumPalIndexadas () const;

    string DevolverFichPalParada () const;

    void ListarPalParada () const;

    int NumPalParada () const;

    string  DevolverDelimitadores () const;

    bool DevolverCasosEspeciales () const;

    bool DevolverPasarAminuscSinAcentos () const;

    bool DevolverAlmacenarPosTerm () const;

    string DevolverDirIndice () const;

    int DevolverTipoStemming () const;

    bool DevolverAlmEnDisco () const;

    void ListarInfColeccDocs () const;

    void ListarTerminos () const;

    bool ListarTerminos (const string&) const;

    void ListarDocs () const;

    bool ListarDocs (const string&) const;
};

#endif PRACTICA_2_INDEXADORHASH_H