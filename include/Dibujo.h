#ifndef DIBUJO_H
#define DIBUJO_H
#include "iostream"
#include <allegro.h>
#include "sad.h"
#include "tuple"

class Dibujo
{
    public:
        Dibujo();
        Dibujo(const char *file, BITMAP * buffer, Coordenada x, Coordenada y);
        virtual void dibujar();
        void activar(){mostrar = true;};
        void desactivar(){mostrar = false;};
        void setImagen(const char * file);
        void updateX(Coordenada x){this->x = x;};
        void updateY(Coordenada y){this->y = y;};
        std::tuple<int,int> puntoMedio();
        std::tuple<Coordenada,Coordenada> getCoordenadas();
        std::tuple<int,int> getTamImagen();
        virtual ~Dibujo();
    protected:
        BITMAP * imagen;
        BITMAP * buffer;
        bool mostrar;
        Coordenada x;
        Coordenada y;
    private:

};

std::tuple<int,int> Dibujo::getTamImagen(){
    return std::make_tuple(imagen->w,imagen->h);
}

std::tuple<Coordenada,Coordenada> Dibujo::getCoordenadas(){
    return std::make_tuple(x,y);
}

std::tuple<int,int> Dibujo::puntoMedio(){
    return std::make_tuple(int(x + (imagen->w / 2)),int(y + (imagen->h / 2)));
}

void Dibujo::setImagen(const char * file){
    imagen = load_bitmap(file,NULL);
}

void Dibujo::dibujar(){
    if(mostrar and imagen and buffer) draw_sprite(buffer,imagen,x,y);
}

Dibujo::Dibujo(const char* file, BITMAP* buffer,Coordenada x, Coordenada y){
    setImagen(file);
    this->buffer = buffer;
    this->x = x;
    this->y = y;
    buffer = nullptr;
    mostrar = true;
}

Dibujo::~Dibujo(){
    destroy_bitmap(imagen);
}

Dibujo::Dibujo(){
    imagen = nullptr;
    mostrar = false;
    x = 0;
    y = 0;
}

#endif // DIBUJO_H
