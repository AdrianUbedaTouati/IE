#ifndef PRACTICA_2_INDEXADORINFORMACION_H
#define PRACTICA_2_INDEXADORINFORMACION_H

class IndexadorInformacion{

    class InformacionTermino {
            friend ostream& operator<<(ostream& s, const InformacionTermino& p);
            ostream& operator<<(ostream& s, const InformacionTermino& p) {
                s << "Frecuencia total: " << p.ftc << "\tfd: " << p.l_docs.size();
                /* A continuación se mostrarían todos los elementos de p.l_docs: s <<
                 * “\tId.Doc: “ << idDoc << “\t” << InfTermDoc;
                 * */
                return s;
            }
        public:
            InformacionTermino (const InformacionTermino &);
            // Inicializa ftc = 0
            InformacionTermino ();
            // Pone ftc = 0 y vacía l_docs
            ~InformacionTermino ();

            // Añadir cuantos métodos se consideren necesarios para manejar la parte privada de la clase
            InformacionTermino & operator= (const InformacionTermino &);

        private:
            // Frecuencia total del término en la colección
            int ftc;

            /* Tabla Hash que se accederá por el id del documento, devolviendo un
             * objeto de la clase InfTermDoc que contiene toda la información de
             * aparición del término en el documento
             * */
            unordered_map<int, InfTermDoc> l_docs;
    };

    class InfTermDoc {
        friend ostream &operator<<(ostream &s, const InfTermDoc &p);

        ostream& operator<<(ostream& s, const InfTermDoc& p) {
            s << "ft: " << p.ft;
            /* A continuación se mostrarían todos los elementos de p.posTerm
             * (“posicion TAB posicion TAB ... posicion,
             * es decir nunca finalizará en un TAB”): s <<“\t“ << posicion;
             * */
            return s;
        }

    public:
        InfTermDoc(const InfTermDoc &);

        // Inicializa ft = 0
        InfTermDoc();

        // Pone ft = 0
        ~InfTermDoc();

        InfTermDoc &operator=(const InfTermDoc &);

    private:
        // Añadir cuantos métodos se consideren necesarios para manejar la parte privada de la clase

        // Frecuencia del término en el documento
        int ft;

        /* Solo se almacenará esta información si el campo privado del indexador almacenarPosTerm == true
         * Lista de números de palabra en los que aparece el término en el
         * documento. Los números de palabra comenzarán desde cero (la primera
         * palabra del documento). Se numerarán las palabras de parada. Estará
         * ordenada de menor a mayor posición.
         * */
        list<int> posTerm;
    };

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

    class InformacionTerminoPregunta {
        friend ostream& operator<<(ostream& s, const InformacionTerminoPregunta& p);
        ostream& operator<<(ostream& s, const InformacionTerminoPregunta& p) {
            s << "ft: " << p.ft;
            // A continuación se mostrarían todos los elementos de p.posTerm (“posicion
            // TAB posicion TAB ... posicion, es decir nunca finalizará en un TAB”): s <<
            // “\t“ << posicion;
            return s;
        }
    public:
        InformacionTerminoPregunta (const InformacionTerminoPregunta &);
        InformacionTerminoPregunta ();
        ~InformacionTerminoPregunta ();
        InformacionTerminoPregunta & operator= (const InformacionTerminoPregunta &);

    private:
        // Añadir cuantos métodos se consideren necesarios para manejar la parte privada de la clase

        // Frecuencia total del término en la pregunta
        int ft;

        /* Solo se almacenará esta información si el campo privado del indexador almacenarPosTerm == true
         * Lista de números de palabra en los que aparece el término en la pregunta.
         * Los números de palabra comenzarán desde cero (la primera
         * palabra de la pregunta). Se numerarán las palabras de parada. Estará
         * ordenada de menor a mayor posición.
         * */
        list<int> posTerm;
    };

    class InformacionPregunta {
        friend ostream& operator<<(ostream& s, const InformacionPregunta& p);

        ostream& operator<<(ostream&, const InformacionPregunta&){
            s << "numTotalPal: " << p.numTotalPal << "\tnumTotalPalSinParada: "<<
              p.numTotalPalSinParada << "\tnumTotalPalDiferentes: " << numTotalPalDiferentes;
            return s;
        }
    public:
        InformacionPregunta (const InformacionPregunta &);
        InformacionPregunta ();
        ~InformacionPregunta ();
        InformacionPregunta & operator= (const InformacionPregunta &);

    private:
        // Añadir cuantos métodos se consideren necesarios para manejar la parte privada de la clase

        // Nº total de palabras en la pregunta
        int numTotalPal;

        // Nº total de palabras sin stop-words en la pregunta
        int numTotalPalSinParada;

        /* Nº total de palabras diferentes en la pregunta que no sean stop-words
         * (sin acumular la frecuencia de cada una de ellas)
         * */
        int numTotalPalDiferentes;
    };
};

#endif //PRACTICA_2_INDEXADORINFORMACION_H
