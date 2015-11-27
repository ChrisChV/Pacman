#ifndef TABLERO_H
#define TABLERO_H
#include "IntervalTree.h"
#include "Seres.h"
#include <iostream>

int ANCHO_VENTANA = 1000;
int ALTO_VENTANA = 700;

const char * __filePacman__Derecha = "pac_der.pcx";
const char * __filePacman__Arriba = "pac_arr.pcx";
const char * __filePacman__Izquierda = "pac_izq.pcx";
const char * __filePacman__Abajo = "pac_abj.pcx";
const int _pacman_vel  = 1;

const char * __fileFantasmita__Izquierda = "fan";
const char * __fileFantasmita__Arriba = "fan";
const char * __fileFantasmita__Derecha = "fan";
const char * __fileFantasmita__Abajo = "fan";


enum Direcciones{HORIZONTAL, VERTICAL};




class Tablero
{
    public:
        class Intersecciones{
            public:
                Intersecciones();
                Intersecciones(Coordenada x, Coordenada y);
                Intersecciones * vecinos[4];
                int x;
                int y;
        };
        class Caminos{
            public:
                Caminos();
                Caminos(Intersecciones * in1, Intersecciones * in2, bool dir, Dis_Intersects dis);
                Intersecciones * inters[2];
                Intersecciones * getMenor(Coordenada coor);
                Intersecciones * getMayor(Coordenada coor);
                Intersecciones * findInter(Coordenada x, Coordenada y); ///Con puntos medios;
                Dis_Intersects distancia;
                bool direccion;
                Coordenada a;
                Coordenada b;
        };
        class Pacman:public Seres{
            public:
                Pacman();
                Pacman(Coordenada x, Coordenada y);
                void dibujar();
                BITMAP * getBuffer(){return this->buffer;};
                virtual ~Pacman();
                bool estoyEnInter();
                Caminos * caminoPacActual;
                Intersecciones * interDestinoPacActual;
        };
        class Fantasmita:public Seres{
            public:
                Fantasmita();
                Fantasmita(BITMAP * buffer, Coordenada x, Coordenada y,Velocidad vel, Pacman * p);
                virtual ~Fantasmita();
                Pacman * pac;

        };
        Tablero();
        Tablero(std::string *m, int fil, int col, const char* file, BITMAP * buffer);
        Caminos * findCamino(Coordenada coor, std::list<IntervalTree<Coordenada,Caminos*>::Nodo *> res); ///Busca el camino correcto con una coordenada de cambio;
        void dibujarTablero();
        void dibujarPacman();
        void actualizarPacman();
        void actualizarPosPacman();
        void actualizarDireccionPacman(Dir_Ser);
        IntervalTree<Coordenada,Caminos *> intervals[2];
        //IntervalTree<Coordenada,Caminos *> intervalVerticales;
        std::list<Dibujo *> paredes;
        virtual ~Tablero();
    protected:
    private:
        int fil;
        int col;
        BITMAP * buffer;
        BITMAP * bufferPac;
        Pacman * pac;

};

void Tablero::actualizarPacman(){
    if(pac->estoyEnInter()){
        auto med = pac->puntoMedio();
        auto cor = pac->getCoordenadas();
        Coordenada corX = std::get<0>(med) - std::get<0>(cor);
        Coordenada corY = std::get<0>(med) - std::get<0>(cor);
        pac->updateX(pac->interDestinoPacActual->x - corX);
        pac->updateY(pac->interDestinoPacActual->y - corY);
        switch(pac->proximaDir){
            case DIR_IZQUIERDA:
                pac->cambiarDir(DIR_IZQUIERDA);
                pac->avanzar();
                actualizarPosPacman();
                break;
            case DIR_DERECHA:
                pac->cambiarDir(DIR_DERECHA);
                pac->avanzar();
                actualizarPosPacman();
                break;
            case DIR_ARRIBA:
                pac->cambiarDir(DIR_ARRIBA);
                pac->avanzar();
                actualizarPosPacman();
                break;
            case DIR_ABAJO:
                pac->cambiarDir(DIR_ABAJO);
                pac->avanzar();
                actualizarPosPacman();
                break;
            case NO_HAY_DIR:
                if(pac->interDestinoPacActual->vecinos[pac->direccion]){
                    pac->avanzar();
                    actualizarPosPacman();
                }
        }
    }
    else{
        pac->avanzar();
    }
    dibujarPacman();
}

void Tablero::actualizarDireccionPacman(Dir_Ser dir){
    switch(pac->direccion){
        case DIR_DERECHA:
            if(dir == DIR_IZQUIERDA){
                pac->interDestinoPacActual = pac->caminoPacActual->inters[0];
                pac->cambiarDir(DIR_IZQUIERDA);
                pac->proximaDir = NO_HAY_DIR;
                return;
            }
            break;
        case DIR_IZQUIERDA:
            if(dir == DIR_DERECHA){
                pac->interDestinoPacActual = pac->caminoPacActual->inters[1];
                pac->cambiarDir(DIR_DERECHA);
                pac->proximaDir = NO_HAY_DIR;
                return;
            }
            break;
        case DIR_ARRIBA:
            if(dir == DIR_ABAJO){
                pac->interDestinoPacActual = pac->caminoPacActual->inters[1];
                pac->cambiarDir(DIR_ABAJO);
                pac->proximaDir = NO_HAY_DIR;
                return;
            }
            break;
        case DIR_ABAJO:
            if(dir == DIR_ARRIBA){
                pac->interDestinoPacActual = pac->caminoPacActual->inters[0];
                pac->cambiarDir(DIR_ARRIBA);
                pac->proximaDir = NO_HAY_DIR;
                return;
            }
            break;
    }
    switch(dir){
        case DIR_IZQUIERDA:
            if(pac->interDestinoPacActual->vecinos[DIR_IZQUIERDA]){
                pac->proximaDir = DIR_IZQUIERDA;
            }
            return;
        case DIR_DERECHA:
            if(pac->interDestinoPacActual->vecinos[DIR_DERECHA]){
                pac->proximaDir = DIR_DERECHA;
            }
            return;
        case DIR_ARRIBA:
            if(pac->interDestinoPacActual->vecinos[DIR_ARRIBA]){
                pac->proximaDir = DIR_ARRIBA;
            }
            return;
        case DIR_ABAJO:
            if(pac->interDestinoPacActual->vecinos[DIR_ABAJO]){
                pac->proximaDir = DIR_ABAJO;
            }
            return;
    }
}


bool Tablero::Pacman::estoyEnInter(){
    auto med = puntoMedio();
    switch(direccion){
        case DIR_DERECHA:
            if(std::get<0>(med) >= interDestinoPacActual->x)return true;
            return false;
        case DIR_IZQUIERDA:
            if(std::get<0>(med) <= interDestinoPacActual->x)return true;
            return false;
        case DIR_ARRIBA:
            if(std::get<1>(med) <= interDestinoPacActual->y)return true;
            return false;
        case DIR_ABAJO:
            if(std::get<1>(med) >= interDestinoPacActual->y)return true;
            return false;
    }
}

void Tablero::actualizarPosPacman(){
    auto med = pac->puntoMedio();
    int temp;
    if(pac->direccion == DIR_DERECHA or pac->direccion == DIR_IZQUIERDA){
        std::cout<<"hola"<<std::endl;
        intervals[0]._print();
        pac->caminoPacActual = findCamino(std::get<1>(med),intervals[0].find(std::get<0>(med),std::get<0>(med)));
    }
    else{
    std::cout<<"adios"<<std::endl;
        pac->caminoPacActual = findCamino(std::get<0>(med),intervals[1].find(std::get<1>(med),std::get<1>(med)));
    }
    std::cout<<"A->"<<pac->caminoPacActual->a<<std::endl;
    std::cout<<"B->"<<pac->caminoPacActual->b<<std::endl;
    if(pac->direccion == DIR_DERECHA or pac->direccion == DIR_ARRIBA){
        pac->interDestinoPacActual = pac->caminoPacActual->inters[1];
    }
    else{
        pac->interDestinoPacActual = pac->caminoPacActual->inters[0];
    }
}

void Tablero::dibujarPacman(){
    pac->dibujar();
    auto tam = pac->getTamImagen();
    auto coor = pac->getCoordenadas();
    std::cout<<"X->"<<std::get<0>(coor)<<std::endl;
    std::cout<<"Y->"<<std::get<1>(coor)<<std::endl;
    blit(bufferPac,screen,0,0,std::get<0>(coor),std::get<1>(coor),std::get<0>(coor),std::get<1>(coor));
}

Tablero::Intersecciones * Tablero::Caminos::findInter(Coordenada x, Coordenada y){
    if(inters[0]->x == x and inters[0]->y == y)return inters[0];
    if(inters[1]->x == x and inters[1]->y == y)return inters[1];
    return nullptr;
}

Tablero::Caminos * Tablero::findCamino(Coordenada coor, std::list<IntervalTree<Coordenada,Caminos*>::Nodo *> res){
    for(IntervalTree<Coordenada,Caminos*>::Nodo * n : res){
        float ff = n->in;
        if(n->in == coor) return n->valor;
    }
    return nullptr;
}

void Tablero::dibujarTablero(){
    for(Dibujo * d : paredes){
        d->dibujar();
    }
    blit(buffer,screen,0,0,0,0,ANCHO_VENTANA,ALTO_VENTANA);
}

Tablero::Intersecciones * Tablero::Caminos::getMayor(Coordenada coor){
    return inters[coor - a < b - coor];
}

Tablero::Intersecciones * Tablero::Caminos::getMenor(Coordenada coor){
    return inters[coor - a > b - coor];
}

Tablero::~Tablero(){

}

Tablero::Tablero(){
    fil = 0;
    col = 0;
}

Tablero::Tablero(std::string * m, int fil, int col, const char * paredFile, BITMAP * buffer){
    this->fil = fil;
    this->col = col;
    this->buffer = buffer;
    BITMAP * pared = load_bitmap(paredFile,NULL);
    int largoPared = pared->w;
    int largoMedia = largoPared / 2;
    int altoPared = pared->h;
    int altoMedia = altoPared / 2;
    Coordenada coordenadaY = altoMedia;
    for(int i = 0; i < fil; i++){
        bool flag = false;
        Intersecciones * inter1 = nullptr;
        Intersecciones * inter2 = nullptr;
        Dis_Intersects camino = 0;
        Coordenada coordenadaX = largoMedia;
        for(int j = 0; j < col; j++){
            if((m[i][j] == ' ' or m[i][j] == 'c') and !flag){
                if(m[i][j] == 'c'){
                    pac = new Pacman(coordenadaX - largoMedia, coordenadaY - altoMedia);
                    bufferPac = pac->getBuffer();
                }
                inter1 = new Intersecciones(coordenadaX,coordenadaY);
                coordenadaX += largoPared;
                flag = true;
                camino ++;
            }
            else if((m[i][j] == ' ' or m[i][j] == 'c') and flag and (m[i-1][j] == ' ' or m[i+1][j] == ' ')){
                if(m[i][j] == 'c'){
                    pac = new Pacman(coordenadaX - largoMedia, coordenadaY - altoMedia);
                    bufferPac = pac->getBuffer();
                }
                if(camino > 0){
                    inter2 = new Intersecciones(coordenadaX,coordenadaY);
                    //std::cout<<"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"<<std::endl;
                    //intervalHorizontales._print();
                    float tt2 = inter2->x;
                    float tt = inter1->x;

                    inter1->vecinos[DIR_DERECHA] = inter2;
                    inter2->vecinos[DIR_IZQUIERDA] = inter1;
                    intervals[HORIZONTAL].insert(inter1->x,inter2->x,inter1->y,new Caminos(inter1,inter2,HORIZONTAL,camino));
                    //std::cout<<"hola->"<<intervalHorizontales.size()<<std::endl;
                    inter1 = inter2;
                }
                else{
                    inter1 = nullptr;
                }
                coordenadaX += largoPared;
                camino = 1;
            }
            else if((m[i][j] == ' ' or m[i][j] == 'c') and flag){
                if(m[i][j] == 'c'){
                    pac = new Pacman(coordenadaX - largoMedia, coordenadaY - altoMedia);
                    bufferPac = pac->getBuffer();
                }
                camino++;
                coordenadaX += largoPared;
            }
            else if((m[i][j] == 'x') and flag){
                if(camino > 1){
                    inter2 = new Intersecciones(coordenadaX - largoPared,coordenadaY);
                    inter1->vecinos[DIR_DERECHA] = inter2;
                    inter2->vecinos[DIR_IZQUIERDA] = inter1;
                    intervals[HORIZONTAL].insert(inter1->x,inter2->x,inter1->y,new Caminos(inter1,inter2,HORIZONTAL,camino));

                }
                else{
                    inter1 = nullptr;
                }
                paredes.push_back(new Dibujo(paredFile,buffer,coordenadaX - largoMedia,coordenadaY - altoMedia));
                coordenadaX += largoPared;
                camino = 0;
                flag = false;
            }
            else if(m[i][j] == 'x' and !flag){
                paredes.push_back(new Dibujo(paredFile,buffer,coordenadaX - largoMedia,coordenadaY - altoMedia));
                coordenadaX += largoPared;
            }
        }
        if(flag and (coordenadaX - largoPared != inter1->x)){
            inter2 = new Intersecciones(coordenadaX - largoPared,coordenadaY);
            inter1->vecinos[DIR_DERECHA] = inter2;
            inter2->vecinos[DIR_IZQUIERDA] = inter1;
            intervals[HORIZONTAL].insert(inter1->x,inter2->x,inter1->y,new Caminos(inter1,inter2,HORIZONTAL,camino));
        }
        coordenadaY += altoPared;
    }
    Coordenada coordenadaX = largoMedia;
    std::cout<<"TAMP_HORIZONTAL->"<<intervals[HORIZONTAL].size()<<std::endl;
    for(int i = 0; i < col; i++){
        bool flag = false;
        Intersecciones * inter1 = nullptr;
        Intersecciones * inter2 = nullptr;
        Dis_Intersects camino = 0;
        coordenadaY = altoMedia;
        for(int j = 0; j < fil; j++){
            if((m[j][i] == ' ' or m[j][i] == 'c') and !flag){
                std::list<IntervalTree<Coordenada,Caminos *>::Nodo *> res = intervals[HORIZONTAL].find(coordenadaX,coordenadaX);
                if(res.empty()){
                    inter1 = new Intersecciones(coordenadaX,coordenadaY);
                }
                else{
                    int sisss = res.size();
                    Caminos * temp = findCamino(coordenadaY,res);
                    float tt = temp->a;
                    float tt2 = temp->b;
                    inter1 = findCamino(coordenadaY,res)->findInter(coordenadaX,coordenadaY);

                }
                coordenadaY += altoPared;
                flag = true;
                camino ++;
            }
            else if((m[j][i] == ' ' or m[j][i] == 'c') and flag and (m[j][i-1] == ' ' or m[j][i+1] == ' ')){
                ///intervalHorizontales._print();
                std::list<IntervalTree<Coordenada,Caminos *>::Nodo *> res = intervals[HORIZONTAL].find(coordenadaX,coordenadaX);
                if(res.empty()){
                    if(camino > 0){
                        inter2 = new Intersecciones(coordenadaX,coordenadaY);
                        inter1->vecinos[DIR_ABAJO] = inter2;
                        inter2->vecinos[DIR_ARRIBA] = inter1;
                        intervals[VERTICAL].insert(inter1->y,inter2->y,inter1->x,new Caminos(inter1,inter2,VERTICAL,camino));
                        inter1 = inter2;
                    }
                    else{
                        inter1 = nullptr;
                    }
                }
                else{
                    if(camino > 0){
                        inter2 = findCamino(coordenadaY,res)->findInter(coordenadaX,coordenadaY);
                        float tt = inter1->y;
                        float tt2 = inter2->y;
                        float tt3 = inter1->x;
                        float tt4 = inter2->x;
                        inter1->vecinos[DIR_ABAJO] = inter2;
                        inter2->vecinos[DIR_ARRIBA] = inter1;
                        intervals[VERTICAL].insert(inter1->y,inter2->y,inter2->x,new Caminos(inter1,inter2,VERTICAL,camino));
                        inter1 = inter2;
                    }
                    else{
                        inter1 = nullptr;
                    }
                }
                coordenadaY += altoPared;
                camino = 1;
            }
            else if((m[j][i] == ' ' or m[j][i] == 'c') and flag){
                camino++;
                coordenadaY += altoPared;
            }
            else if(m[j][i] == 'x' and flag){
                std::list<IntervalTree<Coordenada,Caminos *>::Nodo *> res = intervals[HORIZONTAL].find(coordenadaX,coordenadaX);
                if(res.empty()){
                    if(camino > 1){
                        inter2 = new Intersecciones(coordenadaX,coordenadaY - altoPared);
                        inter1->vecinos[DIR_ABAJO] = inter2;
                        inter2->vecinos[DIR_ARRIBA] = inter1;
                        intervals[VERTICAL].insert(inter1->y,inter2->y,inter1->x,new Caminos(inter1,inter2,VERTICAL,camino));
                    }
                    else{
                        inter1 = nullptr;
                    }
                }
                else{
                    if(camino > 1){
                        inter2 = findCamino(coordenadaY,res)->findInter(coordenadaX,coordenadaY - altoPared);
                        float tt = inter1->y;
                        float tt2 = inter2->y;
                        float tt3 = inter1->x;
                        float tt4 = inter2->x;
                        inter1->vecinos[DIR_ABAJO] = inter2;
                        inter2->vecinos[DIR_ARRIBA] = inter1;
                        intervals[VERTICAL].insert(inter1->y,inter2->y,inter2->x,new Caminos(inter1,inter2,VERTICAL,camino));
                    }
                    else{
                        inter1 = nullptr;
                    }
                }
                coordenadaY += altoPared;
                camino = 0;
                flag = false;
            }
            else if(m[j][i] == 'x' and !flag){
                coordenadaY += altoPared;
            }
        }
        if(flag and (coordenadaY - altoPared != inter1->y)){
            std::list<IntervalTree<Coordenada,Caminos *>::Nodo *> res = intervals[HORIZONTAL].find(coordenadaX,coordenadaX);
            if(res.empty()){
                inter2 = new Intersecciones(coordenadaX,coordenadaY - altoPared);
            }
            else{
                inter2 = findCamino(coordenadaY,res)->findInter(coordenadaX,coordenadaY - altoPared);
            }
            inter1->vecinos[DIR_ABAJO] = inter2;
            inter2->vecinos[DIR_ARRIBA] = inter1;
            intervals[VERTICAL].insert(inter1->y,inter2->y,inter1->x,new Caminos(inter1,inter2,VERTICAL,camino));
        }
        coordenadaX += largoPared;
    }
    std::cout<<"TAM_VERTICAL->"<<intervals[VERTICAL].size()<<std::endl;

}

Tablero::Intersecciones::Intersecciones(Coordenada x, Coordenada y){
    this->x = x;
    this->y = y;
    vecinos[DIR_IZQUIERDA] = nullptr;
    vecinos[DIR_ARRIBA] = nullptr;
    vecinos[DIR_DERECHA] = nullptr;
    vecinos[DIR_ABAJO] = nullptr;
}

Tablero::Intersecciones::Intersecciones(){
    x = -1;
    y = -1;
    vecinos[DIR_IZQUIERDA] = nullptr;
    vecinos[DIR_ARRIBA] = nullptr;
    vecinos[DIR_DERECHA] = nullptr;
    vecinos[DIR_ABAJO] = nullptr;
}

Tablero::Caminos::Caminos(){
    inters[0] = nullptr;
    inters[1] = nullptr;
    direccion = HORIZONTAL;
}

Tablero::Caminos::Caminos(Intersecciones * in1, Intersecciones * in2, bool dir, Dis_Intersects dis){
    direccion = dir;
    distancia = dis;
    if(dir == HORIZONTAL){
        if(in1->x > in2->x){
            a = in2->x;
            b = in1->x;
            inters[0] = in2;
            inters[1] = in1;
        }
        else{
            a = in1->x;
            b = in2->x;
            inters[0] = in1;
            inters[1] = in2;
        }
    }
    else{
        if(in1->y > in2->y){
            a = in2->y;
            b = in1->y;
            inters[0] = in2;
            inters[1] = in1;
        }
        else{
            a = in1->y;
            b = in2->y;
            inters[0] = in1;
            inters[1] = in2;
        }
    }
}

void Tablero::Pacman::dibujar(){
    if(mostrar and imagen and buffer) draw_sprite(buffer,imagen,0,0);
}

Tablero::Pacman::~Pacman(){
    destroy_bitmap(serIzquierda);
    destroy_bitmap(serDerecha);
    destroy_bitmap(serArriba);
    destroy_bitmap(serAbajo);
    destroy_bitmap(imagen);
}

Tablero::Pacman::Pacman(Coordenada x, Coordenada y){
    serIzquierda = load_bitmap(__filePacman__Izquierda,NULL);
    serDerecha = load_bitmap(__filePacman__Derecha,NULL);
    serArriba = load_bitmap(__filePacman__Arriba,NULL);
    serAbajo = load_bitmap(__filePacman__Abajo,NULL);
    imagen = serIzquierda;
    buffer = create_bitmap(imagen->w,imagen->h);
    clear_to_color(buffer, 0x999999);
    direccion = DIR_IZQUIERDA;
    proximaDir = NO_HAY_DIR;
    this->x = x;
    this->y = y;
    this->velocidad = _pacman_vel;
    mostrar = true;
}

Tablero::Pacman::Pacman(){
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

Tablero::Fantasmita::~Fantasmita(){
    destroy_bitmap(serIzquierda);
    destroy_bitmap(serDerecha);
    destroy_bitmap(serArriba);
    destroy_bitmap(serAbajo);
    destroy_bitmap(imagen);
}

Tablero::Fantasmita::Fantasmita(BITMAP * buffer, Coordenada x, Coordenada y,Velocidad vel, Pacman * p){
    this->buffer = buffer;
    pac = p;
    this->velocidad = vel;
    serIzquierda = load_bitmap(__filePacman__Izquierda,NULL);
    serDerecha = load_bitmap(__filePacman__Derecha,NULL);
    serArriba = load_bitmap(__filePacman__Arriba,NULL);
    serAbajo = load_bitmap(__filePacman__Abajo,NULL);
    imagen = serDerecha;
    direccion = DIR_DERECHA;
    proximaDir = NO_HAY_DIR;
    this->x = x;
    this->y = y;
    mostrar = true;
}

Tablero::Fantasmita::Fantasmita(){
    imagen = nullptr;
    serAbajo = nullptr;
    serArriba = nullptr;
    serDerecha = nullptr;
    serIzquierda = nullptr;
    pac = nullptr;
    buffer = nullptr;
    mostrar = false;
    x = 0;
    y = 0;
}

#endif // TABLERO_H
