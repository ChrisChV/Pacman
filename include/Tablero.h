#ifndef TABLERO_H
#define TABLERO_H
#include "IntervalTree.h"
#include "Dibujo.h"
#include <iostream>

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
                Intersecciones * findInter(Coordenada x, Coordenada y);
                Dis_Intersects distancia;
                bool direccion;
                Coordenada a;
                Coordenada b;
        };
        Tablero();
        Tablero(std::string *m, int fil, int col, const char* file, BITMAP * buffer);
        Caminos * findCamino(Coordenada coor, std::list<IntervalTree<Coordenada,Caminos*>::Nodo *> &res);
        int fil;
        int col;
        BITMAP * buffer;
        void dibujarTablero();
        IntervalTree<Coordenada,Caminos *> intervalHorizontales;
        IntervalTree<Coordenada,Caminos *> intervalVerticales;
        std::list<Dibujo *> paredes;
        virtual ~Tablero();
    protected:
    private:
};

Tablero::Intersecciones * Tablero::Caminos::findInter(Coordenada x, Coordenada y){
    if(inters[0]->x == x and inters[0]->y == y)return inters[0];
    if(inters[1]->x == x and inters[1]->y == y)return inters[1];
    return nullptr;
}

Tablero::Caminos * Tablero::findCamino(Coordenada coor, std::list<IntervalTree<Coordenada,Caminos*>::Nodo *> &res){
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
            if(m[i][j] == ' ' and !flag){
                inter1 = new Intersecciones(coordenadaX,coordenadaY);
                coordenadaX += largoPared;
                flag = true;
                camino ++;
            }
            else if(m[i][j] == ' ' and flag and (m[i-1][j] == ' ' or m[i+1][j] == ' ')){
                if(camino > 0){
                    inter2 = new Intersecciones(coordenadaX,coordenadaY);
                    //std::cout<<"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"<<std::endl;
                    //intervalHorizontales._print();
                    float tt2 = inter2->x;
                    float tt = inter1->x;

                    inter1->vecinos[DIR_DERECHA] = inter2;
                    inter2->vecinos[DIR_IZQUIERDA] = inter1;
                    intervalHorizontales.insert(inter1->x,inter2->x,inter1->y,new Caminos(inter1,inter2,HORIZONTAL,camino));
                    //std::cout<<"hola->"<<intervalHorizontales.size()<<std::endl;
                    inter1 = inter2;
                }
                else{
                    inter1 = nullptr;
                }
                coordenadaX += largoPared;
                camino = 1;
            }
            else if(m[i][j] == ' ' and flag){
                camino++;
                coordenadaX += largoPared;
            }
            else if((m[i][j] == 'x') and flag){
                if(camino > 1){
                    inter2 = new Intersecciones(coordenadaX - largoPared,coordenadaY);
                    std::cout<<"hola->"<<intervalHorizontales.size()<<std::endl;
                    inter1->vecinos[DIR_DERECHA] = inter2;
                    inter2->vecinos[DIR_IZQUIERDA] = inter1;
                    intervalHorizontales.insert(inter1->x,inter2->x,inter1->y,new Caminos(inter1,inter2,HORIZONTAL,camino));

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
            intervalHorizontales.insert(inter1->x,inter2->x,inter1->y,new Caminos(inter1,inter2,HORIZONTAL,camino));
        }
        coordenadaY += altoPared;
    }
    std::cout<<intervalHorizontales.size()<<std::endl;
    intervalHorizontales._print();
    intervalHorizontales.verificarMax(intervalHorizontales.root);
    Coordenada coordenadaX = largoMedia;
    for(int i = 0; i < col; i++){
        bool flag = false;
        Intersecciones * inter1 = nullptr;
        Intersecciones * inter2 = nullptr;
        Dis_Intersects camino = 0;
        coordenadaY = altoMedia;
        for(int j = 0; j < fil; j++){
            if(m[j][i] == ' ' and !flag){
                std::list<IntervalTree<Coordenada,Caminos *>::Nodo *> res = intervalHorizontales.find(coordenadaX,coordenadaX);
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
            else if(m[j][i] == ' ' and flag and (m[j][i-1] == ' ' or m[j][i+1] == ' ')){
                std::cout<<"AAAAAAAAAAAAAAAAAAAAA"<<std::endl;
                ///intervalHorizontales._print();
                std::list<IntervalTree<Coordenada,Caminos *>::Nodo *> res = intervalHorizontales.find(coordenadaX,coordenadaX);
                if(res.empty()){
                    if(camino > 0){
                        inter2 = new Intersecciones(coordenadaX,coordenadaY);
                        inter1->vecinos[DIR_ABAJO] = inter2;
                        inter2->vecinos[DIR_ARRIBA] = inter1;
                        intervalVerticales.insert(inter1->y,inter2->y,inter1->x,new Caminos(inter1,inter2,VERTICAL,camino));
                        inter1 = inter2;
                        std::cout<<"hola22222->"<<intervalVerticales.size()<<std::endl;
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
                        intervalVerticales.insert(inter1->y,inter2->y,inter2->x,new Caminos(inter1,inter2,VERTICAL,camino));
                        inter1 = inter2;
                        std::cout<<"hola3333->"<<intervalVerticales.size()<<std::endl;
                    }
                    else{
                        inter1 = nullptr;
                    }
                }
                coordenadaY += altoPared;
                camino = 1;
            }
            else if(m[j][i] == ' ' and flag){
                camino++;
                coordenadaY += altoPared;
            }
            else if(m[j][i] == 'x' and flag){
                std::list<IntervalTree<Coordenada,Caminos *>::Nodo *> res = intervalHorizontales.find(coordenadaX,coordenadaX);
                if(res.empty()){
                    if(camino > 1){
                        inter2 = new Intersecciones(coordenadaX,coordenadaY - altoPared);
                        inter1->vecinos[DIR_ABAJO] = inter2;
                        inter2->vecinos[DIR_ARRIBA] = inter1;
                        intervalVerticales.insert(inter1->y,inter2->y,inter1->x,new Caminos(inter1,inter2,VERTICAL,camino));
                        std::cout<<"adios"<<intervalVerticales.size()<<std::endl;
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
                        intervalVerticales.insert(inter1->y,inter2->y,inter2->x,new Caminos(inter1,inter2,VERTICAL,camino));
                        std::cout<<"i->"<<i<<std::endl;
                        std::cout<<"hola->"<<intervalVerticales.size()<<std::endl;
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
            std::list<IntervalTree<Coordenada,Caminos *>::Nodo *> res = intervalHorizontales.find(coordenadaX,coordenadaX);
            if(res.empty()){
                inter2 = new Intersecciones(coordenadaX,coordenadaY - altoPared);
            }
            else{
                inter2 = findCamino(coordenadaY,res)->findInter(coordenadaX,coordenadaY - altoPared);
            }
            inter1->vecinos[DIR_ABAJO] = inter2;
            inter2->vecinos[DIR_ARRIBA] = inter1;
            intervalVerticales.insert(inter1->y,inter2->y,inter1->x,new Caminos(inter1,inter2,VERTICAL,camino));
        }
        coordenadaX += largoPared;
    }
    std::cout<<intervalVerticales.size()<<std::endl;

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

#endif // TABLERO_H
