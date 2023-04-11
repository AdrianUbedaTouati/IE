#ifndef PRACTICA_2_INFORMACIONTERMINOPREGUNTA_H
#define PRACTICA_2_INFORMACIONTERMINOPREGUNTA_H


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

#endif //PRACTICA_2_INFORMACIONTERMINOPREGUNTA_H
