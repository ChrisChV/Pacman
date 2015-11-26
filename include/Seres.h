#ifndef SERES_H
#define SERES_H
#include "Dibujo.h"

enum Dir{DIR_IZQUIERDA,DIR_ARRIBA,DIR_ABAJO,DIR_DERECHA}

class Seres:public Dibujo
{
    public:
        Seres();
        virtual ~Seres();
    protected:
        BITMAP * serIzquierda;
        BITMAP * serDerecha;
        BITMAP * serArriba;
        BITMAP * serAbajo;
        int direccion;
    private:
};

Seres::Seres(){
    direccion = DIR_DERECHA;
    imagen = nullptr;
    serAbajo = nullptr;
    serArriba = nullptr;
    serDerecha = nullptr;
    serIzquierda = nullptr;
    buffer = nullptr;
    mostrar = false;
    int x = 0;
    int y = 0;
}

#endif // SERES_H
