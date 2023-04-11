#include "../include/indexadorinformacion.h"

/**
 * InfTermDoc
 */

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

ostream& operator<<(ostream& s, const InfDoc& p) {
    s << "idDoc: " << p.idDoc << "\tnumPal: " << p.numPal <<
      "\tnumPalSinParada: " << p.numPalSinParada << "\tnumPalDiferentes: " <<
      p.numPalDiferentes << "\ttamBytes: " << p.tamBytes;
    return s;
}

/**
 * InfColeccionDocs
 */

ostream& operator<<(ostream& s, const InfColeccionDocs& p){
    s << "numDocs: " << p.numDocs << "\tnumTotalPal: " << p.numTotalPal <<
      "\tnumTotalPalSinParada: " << p.numTotalPalSinParada <<
      "\tnumTotalPalDiferentes: " << numTotalPalDiferentes << "\ttamBytes: " <<
      p.tamBytes;
    return s;
}

/**
* InformacionTerminoPregunta
*/

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

ostream& operator<<(ostream& s, const InformacionPregunta& p){
    s << "numTotalPal: " << p.numTotalPal << "\tnumTotalPalSinParada: "<<
      p.numTotalPalSinParada << "\tnumTotalPalDiferentes: " << numTotalPalDiferentes;
    return s;
}