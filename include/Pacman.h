#ifndef PACMAN_H
#define PACMAN_H
#include "Seres.h"

const char * __filePacman__Derecha = "pac.bmp";
const char * __filePacman__Arriba = "pac";
const char * __filePacman__Izquierda = "pac";
const char * __filePacman__Abajo = "pac";
const int velocidad  = 1;

class Pacman:public Seres
{
    public:
        Pacman();
        Pacman(BITMAP * buffer, Coordenada x, Coordenada y);
        virtual ~Pacman();
    protected:
    private:
};

Pacman::~Pacman(){
    destroy_bitmap(serIzquierda);
    destroy_bitmap(serDerecha);
    destroy_bitmap(serArriba);
    destroy_bitmap(serAbajo);
    destroy_bitmap(imagen);
}

Pacman::Pacman(BITMAP * buffer, Coordenada x, Coordenada y){
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

Pacman::Pacman(){
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


#endif // PACMAN_H
