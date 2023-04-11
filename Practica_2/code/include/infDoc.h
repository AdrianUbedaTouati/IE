#ifndef PRACTICA_2_INFDOC_H
#define PRACTICA_2_INFDOC_H


class InfDoc {

    friend ostream& operator<<(ostream& s, const InfDoc& p);

    ostream& operator<<(ostream& s, const InfDoc& p) {
        s << "idDoc: " << p.idDoc << "\tnumPal: " << p.numPal <<
          "\tnumPalSinParada: " << p.numPalSinParada << "\tnumPalDiferentes: " <<
          p.numPalDiferentes << "\ttamBytes: " << p.tamBytes;
        return s;
    }

    public:
        InfDoc (const InfDoc &);
        InfDoc ();
        ~InfDoc ();
        InfDoc & operator= (const InfDoc &);

    private:
        // Añadir cuantos métodos se consideren necesarios para manejar la parte privada de la clase

        // Identificador del documento. El primer documento indexado en la colección será el identificador 1
        int idDoc;

        // Nº total de palabras del documento
        int numPal;

        // Nº total de palabras sin stop-words del documento
        int numPalSinParada;

        // Nº total de palabras diferentes que no sean stop-words (sin acumular la frecuencia de cada una de ellas)
        int numPalDiferentes;

        // Tamaño en bytes del documento
        int tamBytes;

        /* Atributo correspondiente a la fecha y hora (completa) de modificación del documento.
         * El tipo “Fecha/hora” lo elegirá/implementará el alumno
         * */
        Fecha fechaModificacion;
};




#endif //PRACTICA_2_INFDOC_H
