#ifndef PRACTICA_2_INFCOLECCIONDOCS_H
#define PRACTICA_2_INFCOLECCIONDOCS_H

class InfColeccionDocs {
    friend ostream& operator<<(ostream& s, const InfColeccionDocs& p);
    ostream& operator<<(ostream&, const InfColeccionDocs&){
        s << "numDocs: " << p.numDocs << "\tnumTotalPal: " << p.numTotalPal <<
        "\tnumTotalPalSinParada: " << p.numTotalPalSinParada <<
        "\tnumTotalPalDiferentes: " << numTotalPalDiferentes << "\ttamBytes: " <<
        p.tamBytes;
        return s;
    }
public:
    InfColeccionDocs (const InfColeccionDocs &);
    InfColeccionDocs ();
    ~InfColeccionDocs ();
    InfColeccionDocs & operator= (const InfColeccionDocs &);

private:
    // Añadir cuantos métodos se consideren necesarios para manejar la parte privada de la clase

    // Nº total de documentos en la colección
    int numDocs;

    // Nº total de palabras en la colección
    int numTotalPal;

    // Nº total de palabras sin stop-words en la colección
    int numTotalPalSinParada;

    /* Nº total de palabras diferentes en la colección que no sean
     * stopwords (sin acumular la frecuencia de cada una de ellas)
     * */
    int numTotalPalDiferentes;

    // Tamaño total en bytes de la colección
    int tamBytes;
};

#endif //PRACTICA_2_INFCOLECCIONDOCS_H
