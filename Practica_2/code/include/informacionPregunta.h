#ifndef PRACTICA_2_INFORMACIONPREGUNTA_H
#define PRACTICA_2_INFORMACIONPREGUNTA_H


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



#endif //PRACTICA_2_INFORMACIONPREGUNTA_H
