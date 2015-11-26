#ifndef PACMAN_H
#define PACMAN_H
#include "Seres.h"

const char * __filePacman__Derecha = "pac.bmp";
const char * __filePacman__Arriba = "pac";
const char * __filePacman__Izquierda = "pac";
const char * __filePacman__Abajo = "pac";

class Pacman:public Seres
{
    public:
        Pacman();
        Pacman(BITMAP * buffer, int x, int y);
        virtual ~Pacman();
    protected:
    private:
};

Pacman::Pacman(BITMAP * buffer, int x, int y){
    this->buffer = buffer;
    serIzquierda = load_bitmap(__filePacman__Izquierda,NULL);
    serDerecha = load_bitmap(__filePacman__Derecha,NULL);
    serArriba = load_bitmap(__filePacman__Arriba,NULL);
    serAbajo = load_bitmap(__filePacman__Abajo,NULL);
    imagen = nullptr;
    this->x = x;
    this->y = y;
    mostrar = true;
}

Pacman::Pacman(){
    imagen = nullptr;
    pacAbajo = nullptr;
    pacArriba = nullptr;
    pacDerecha = nullptr;
    pacIzquierda = nullptr;
    buffer = nullptr;
    mostrar = false;
    int x = 0;
    int y = 0;
}


#endif // PACMAN_H
