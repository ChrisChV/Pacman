#ifndef SERES_H
#define SERES_H
#include "Dibujo.h"



class Seres:public Dibujo
{
    public:
        Seres();
        void avanzar();
        void cambiarDir(Dir_Ser dir);
        bool colision(Seres *);
        virtual ~Seres();
    protected:
        BITMAP * serIzquierda;
        BITMAP * serDerecha;
        BITMAP * serArriba;
        BITMAP * serAbajo;
        Velocidad velocidad;
        Dir_Ser direccion;
    private:
};

bool Seres::colision(Seres * secondSer){
    if (((y<secondSer->y + secondSer->imagen->h && x<secondSer->x + secondSer->imagen->w)
        &&(y<secondSer->y + secondSer->imagen->h && x + imagen->w>secondSer->x)
        &&(y + imagen->h>secondSer->y && x + imagen->w>secondSer->x)
        &&(y + imagen->h>secondSer->y && x<secondSer->x + secondSer->imagen->w)))
        {
            return true;
        }
    return false;
}

void Seres::cambiarDir(Dir_Ser dir){
    switch(dir){
        case DIR_IZQUIERDA:
            direccion = DIR_IZQUIERDA;
            imagen = serIzquierda;
            break;
        case DIR_ARRIBA:
            direccion = DIR_ARRIBA;
            imagen = serArriba;
            break;
        case DIR_DERECHA:
            direccion = DIR_DERECHA;
            imagen = serDerecha;
            break;
        case DIR_ABAJO:
            direccion = DIR_ABAJO;
            imagen = serAbajo;
            break;
    }
}

void Seres::avanzar(){
    switch(direccion){
        case DIR_IZQUIERDA:
            x -= velocidad;
            break;
        case DIR_ARRIBA:
            y -= velocidad;
            break;
        case DIR_DERECHA:
            x += velocidad;
            break;
        case DIR_ABAJO:
            y += velocidad;
            break;
    }
}

Seres::~Seres(){
    destroy_bitmap(serIzquierda);
    destroy_bitmap(serDerecha);
    destroy_bitmap(serArriba);
    destroy_bitmap(serAbajo);
    destroy_bitmap(imagen);
}

Seres::Seres(){
    direccion = DIR_DERECHA;
    imagen = nullptr;
    serAbajo = nullptr;
    serArriba = nullptr;
    serDerecha = nullptr;
    serIzquierda = nullptr;
    buffer = nullptr;
    mostrar = false;
    velocidad = 0;
    x = 0;
    y = 0;
}

#endif // SERES_H
