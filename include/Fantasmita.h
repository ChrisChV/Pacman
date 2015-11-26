#ifndef FANTASMITA_H
#define FANTASMITA_H
#include "Pacman.h"

const char * __fileFantasmita__Izquierda = "fan";
const char * __fileFantasmita__Arriba = "fan";
const char * __fileFantasmita__Derecha = "fan";
const char * __fileFantasmita__Abajo = "fan";

class Fantasmita:public Seres
{
    public:
        Fantasmita();
        Fantasmita(BITMAP * buffer, Coordenada x, Coordenada y);
        virtual ~Fantasmita();
    protected:
    private:
        Pacman * pac;
};

Fantasmita::~Fantasmita(){
    destroy_bitmap(serIzquierda);
    destroy_bitmap(serDerecha);
    destroy_bitmap(serArriba);
    destroy_bitmap(serAbajo);
    destroy_bitmap(imagen);
}

Fantasmita::Fantasmita(BITMAP * buffer, Coordenada x, Coordenada y){
    this->buffer = buffer;
    serIzquierda = load_bitmap(__filePacman__Izquierda,NULL);
    serDerecha = load_bitmap(__filePacman__Derecha,NULL);
    serArriba = load_bitmap(__filePacman__Arriba,NULL);
    serAbajo = load_bitmap(__filePacman__Abajo,NULL);
    imagen = serDerecha;
    direccion = DIR_DERECHA;
    this->x = x;
    this->y = y;
    mostrar = true;
}

Fantasmita::Fantasmita(){
    imagen = nullptr;
    serAbajo = nullptr;
    serArriba = nullptr;
    serDerecha = nullptr;
    serIzquierda = nullptr;
    buffer = nullptr;
    mostrar = false;
    x = 0;
    y = 0;
}

#endif // FANTASMITA_H
