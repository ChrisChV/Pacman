#ifndef DIBUJO_H
#define DIBUJO_H
#include "iostream"
#include <allegro.h>

class Dibujo
{
    public:
        Dibujo();
        Dibujo(const char *file, BITMAP * buffer, int x, int y);
        virtual void dibujar();
        void activar(){mostrar = true;};
        void desactivar(){mostrar = false;};
        void setImagen(const char * file);
        virtual ~Dibujo();
    protected:
        BITMAP * imagen;
        BITMAP * buffer;
        bool mostrar;
        int x;
        int y;
    private:

};

void Dibujo::setImagen(const char * file){
    imagen = load_bitmap(file,NULL);
}

void Dibujo::dibujar(){
    if(mostrar and imagen and buffer) draw_sprite(buffer,imagen,x,y);
}

Dibujo::Dibujo(const char* file, BITMAP* buffer,int x, int y){
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
    int x = 0;
    int y = 0;
}

#endif // DIBUJO_H
