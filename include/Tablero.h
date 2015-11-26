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
                Intersecciones()
                Intersecciones(int fil, int col);
                int col;
                int fil;
        };
        class Caminos{
            public:
                Caminos()
                Caminos(Intersecciones * in1, Intersecciones * in2, bool dir, int dis);
                Intersecciones * inters[2];
                Intersecciones * getMenor(int coor);
                int distancia;
                bool direccion;
                int a;
                int b;
        };
        Tablero();
        Tablero(char ** m, int fil, int col, const char* file, BITMAP * buffer);
        int fil;
        int col;
        BITMAP * buffer;
        IntervalTree<int,Caminos *> intervalHorizontales;
        IntervalTree<int,Caminos *> intervalVerticales;
        std::list<Dibujo *> paredes;
        virtual ~Tablero();
    protected:
    private:
};

Tablero::Intersecciones * Tablero::Caminos::getMenor(int coor){
    return inters[coor - a > b - coor];
}

Tablero::Tablero(){
    fil = 0;
    col = 0;
}

Tablero::Tablero(string * m; int fil, int col, const char * paredFile, BITMAP * buffer){
    this->fil = fil;
    this->col = col;
    this->buffer = buffer;
    BITMAP * pared = load_bitmap(paredFile,NULL);
    int largoPared = pared.w;
    int largoMedia = largoPared / 2;
    int altoPared = pared.h;
    int altoMedia = altoPared / 2;
    int coordenadaY = altoMedia;
    for(int i = 0; i < fil; i++){
        bool flag = false;
        Intersecciones * inter1 = nullptr;
        Intersecciones * inter2 = nullptr;
        int camino = 0;
        int coordenadaX = largoMedia;
        for(int j = 0; j < col; j++){
            if(m[i][j] == 'x' and !flag){
                inter1 = new Intersecciones(coordenadaX,coordenadaY);
                coordenadaX += largoPared;
                flag = true;
                camino ++;
            }
            else if(m[i][j] == 'x' and flag){
                camino++;
                coordenadaX += largoPared;
            }
            else if(m[i][j] == ' ' and flag){
                inter2 = new Intersecciones(coordenadaX - largoPared,coordenadaY);
                coordenadaX += largoPared;
                paredes.push_back(new Dibujo(paredFile,buffer,coordenadaX - largoMedia,coordenadaY - altoMedia));
                intervalHorizontales.insert(inter1->fil,inter1->fil,new Caminos(inter1,inter2,HORIZONTAL,camino));
                camino = 0;
                flag = false;
            }
            else if(m[i][j] == ' ' and !flag){
                paredes.push_back(new Dibujo(paredFile,buffer,coordenadaX - largoMedia,coordenadaY - altoMedia));
                coordenadaX += largoPared;
            }
        }
        if(flag and !(coordenadaX - largoMedia == inter1->fil)){
            inter2 = new Intersecciones(coordenadaX - largoPared,coordenadaY);
            intervalHorizontales.insert(inter1->fil,inter1->fil,new Caminos(inter1,inter2,HORIZONTAL,camino));
        }
        coordenadaY += altoPared;
    }
    int coordenadaX = largoMedia;
    for(int i = 0; i < col; i++){
        bool flag = false;
        Intersecciones * inter1 = nullptr;
        Intersecciones * inter2 = nullptr;
        int camino = 0;
        coordenadaY = altoMedia;
        for(int j = 0; j < fil; j++){
            if(m[i][j] == 'x' and !flag){
                Caminos * temp;
                if(!intervalHorizontales.find(coordenadaX,coordenadaX,temp)){
                    inter1 = new Intersecciones(coordenadaX,coordenadaY);
                }
                else{
                    inter1 = temp->getMenor(coordenadaX);
                }
                coordenadaY += altoPared;
                flag = true;
                camino ++;
            }
            else if(m[i][j] == 'x' and flag){
                camino++;
                coordenadaY += altoPared;
            }
            else if(m[i][j] == ' ' and flag){
                if(!intervalHorizontales.find(coordenadaX,coordenadaX,temp)){
                    inter2 = new Intersecciones(coordenadaX,coordenadaY - altoPared);
                }
                else{
                    inter1 = temp->getMenor(coordenadaX);
                }
                coordenadaY += altoPared;
                intervalVerticales.insert(inter1->col,inter2->col,new Caminos(inter1,inter2,HORIZONTAL,camino));
                camino = 0;
                flag = false;
            }
            else if(m[i][j] == ' ' and !flag){
                coordenadaY += altoPared;
            }
        }
        if(flag and !(coordenadaY - altoMedia == inter1->fil)){
            inter2 = new Intersecciones(coordenadaX - largoPared,coordenadaY);
            intervalVerticales.insert(inter1->col,inter1->col,new Caminos(inter1,inter2,HORIZONTAL,camino));
        }
        coordenadaX += largoPared;
    }
}

Tablero::Intersecciones::Intersecciones(int fil, int col){
    this->fil = fil;
    this->col = col;
}

Tablero::Intersecciones::Intersecciones(){
    fil = -1;
    col = -1;
}

Tablero::Caminos::Caminos(){
    inters[0] = nullptr;
    inters[1] = nullptr;
    direccion = HORIZONTAL;
}

Tablero::Caminos::Caminos(Intersecciones * in1, Intersecciones * in2, bool dir, int dis){
    direccion = dir;
    distancia = dis;
    if(dir == HORIZONTAL){
        if(in1->fil > in2->fil){
            a = in2->fil;
            b = in1->fil;
            inters[0] = in2;
            inters[1] = in1;
        }
        else{
            a = in1->fil;
            b = in2->fil;
            inters[0] = in1;
            inters[1] = in2;
        }
    }
    else{
        if(in1->col > in2->fil){
            a = in2->col;
            b = in1->col;
            inters[0] = in2;
            inters[1] = in1;
        }
        else{
            a = in1->col;
            b = in2->col;
            inters[0] = in1;
            inters[1] = in2;
        }
    }
}

#endif // TABLERO_H
