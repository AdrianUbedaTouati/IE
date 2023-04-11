#ifndef PRACTICA_2_INDEXADORINFORMACION_H
#define PRACTICA_2_INDEXADORINFORMACION_H

class InformacionTermino {
        friend ostream& operator<<(ostream& s, const InformacionTermino& p);
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

ostream& operator<<(ostream& s, const InformacionTermino& p) {
    s << "Frecuencia total: " << p.ftc << "\tfd: " << p.l_docs.size();
    /* A continuación se mostrarían todos los elementos de p.l_docs: s <<
     * “\tId.Doc: “ << idDoc << “\t” << InfTermDoc;
     * */
    return s;
}

#endif //PRACTICA_2_INDEXADORINFORMACION_H
