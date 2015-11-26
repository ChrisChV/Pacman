#ifndef TABLERO_H
#define TABLERO_H

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
                Caminos(Intersecciones * in1, Intersecciones * in2, bool dir);
                Intersecciones * inters[2];
                int distancia;
                bool direccion;
                int a;
                int b;
        };
        Tablero();
        Tablero(char ** m, int fil, int col);
        int fil;
        int col;
        IntervalTree<Caminos *> interval;
        virtual ~Tablero();
    protected:
    private:
};

Tablero::Tablero(){
    fil = 0;
    col = 0;
}

Tablero::Tablero(char ** m; int fil, int col){
    this->fil = fil;
    this->col = col;
    for(int i = 0; i < fil; i++){
        bool flag = false;
        Intersecciones * inter1 = nullptr;
        Intersecciones * inter2 = nullptr;
        int camino = 0;
        for(int j = 0; j < col; j++){
            if(m[i][j] == 'x' and !flag){
                inter1 = new Intersecciones(i,j);
                flag = true;
                camino ++;
            }
            else if(m[i][j] == 'x' and flag) camino++;
            else if(m[i][j] == ' ' and flag){
                inter2 = new Intersecciones(i,j);
                flag = false;
            }
        }
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

Tablero::Caminos::Caminos(Intersecciones * in1, Intersecciones * in2, bool dir){
    inters[0] = in1;
    inters[1] = in2;
    direccion = dir;
    if(dir == HORIZONTAL){
        if(in1->fil > in2->fil){
            a = in2->fil;
            b = in1->fil;
        }
        else{
            a = in1->fil;
            b = in2->fil;
        }
    }
    else{
        if(in1->col > in2->fil){
            a = in2->col;
            b = in1->fil;
        }
        else{
            a = in1->col;
            b = in2->col;
        }
    }
}

#endif // TABLERO_H
