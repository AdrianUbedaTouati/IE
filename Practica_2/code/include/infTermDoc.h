#ifndef PRACTICA_2_INFTERMDOC_H
#define PRACTICA_2_INFTERMDOC_H

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





#endif //PRACTICA_2_INFTERMDOC_H
