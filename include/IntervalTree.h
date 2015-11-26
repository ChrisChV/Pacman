#ifndef INTERVALTREE_H
#define INTERVALTREE_H
#include "fstream"
#include "iostream"
#include "list"

using namespace std;

enum Colores{NEGRO, ROJO};
enum Giros{IZQUIERDA, DERECHA};

template <typename T, typename V>
class IntervalTree
{
    public:
        class Nodo{
            public:
                Nodo();
                Nodo(T,T,V);
                T low;
                T high;
                T max;
                V valor;
                Nodo * hijos[2];
                Nodo * padre;
                bool color;
                void destruirme();
        };
        IntervalTree();
        void insert(T,T,V);
        bool find(T,T, V);
        void print();
        virtual ~IntervalTree();
    protected:
    private:
        Nodo * root;
        int _find(T, Nodo **&, Nodo *&);
        bool __find(T, Nodo **&, Nodo *&);
        void rotacionSimple(Nodo *&, bool);
        void rotacionCompleja(Nodo *&, bool);
        void verificarMax(Nodo *);
        bool find(T,T,Nodo*, V res);

};

template <typename T, typename V>
bool IntervalTree<T>::find(T low, T high, V res){
    return find(low,high,root,res);
}

template <typename T, typename V>
bool IntervalTree<T,V>::find(T low, T high, Nodo *nodo, V res){
    if(!nodo) return false;
    if(low >= nodo->low and high <= nodo->high){
        res = nodo->valor;
        return true;
    }
    else if(nodo->hijos[0] and nodo->hijos[0]->max > nodo->low){
        return find(low,high,nodo->hijos[0],res);
    }
    else return find(low,high,nodo->hijos[1],res);
}

template <typename T, typename V>
void IntervalTree<T,V>::print(){
    ofstream archivo("eje.dot");
    if(archivo.fail()){
        cout<<"EL archivo no se pudo abrir"<<endl;
        return;
    }
    archivo<<"digraph{"<<endl;
    list<Nodo *> result;
    if(root) result.push_back(root);
    while(!result.empty()){
        list<Nodo *> temp;
        for(auto iter = result.begin(); iter != result.end(); ++iter){
            string color;
            if((*iter)->color) color = "red";
            else color = "black";
            archivo<<(*iter)->low * (*iter)->high + 1<<" [label=\"["<<(*iter)->low<<","<<(*iter)->high<<"]"<<endl<<"max = "<<(*iter)->max<<"\", color = "<<color<<"];"<<endl;
            if((*iter)->hijos[0]){
                temp.push_back((*iter)->hijos[0]);
                archivo<<(*iter)->low * (*iter)->high + 1<<"->"<<(*iter)->hijos[0]->low * (*iter)->hijos[0]->high +1<<";"<<endl;
            }
            if((*iter)->hijos[1]){
                temp.push_back((*iter)->hijos[1]);
                archivo<<(*iter)->low * (*iter)->high + 1<<"->"<<(*iter)->hijos[1]->low * (*iter)->hijos[1]->high +1<<";"<<endl;
            }
        }
        result = temp;
    }
    archivo<<"}";
    archivo.close();
    system("dot -Tpdf eje.dot -o eje.pdf");
}

template <typename T, typename V>
void IntervalTree<T,V>::insert(T low, T high, V val){
    Nodo ** nodo;
    Nodo * padre;
    int flag = this->_find(low, nodo, padre);
    if(flag == 1){
        return;
        /*
        if((*nodo)->high == high){
            return;
        }
        Nodo ** nodo;
        Nodo * padre;
        __find(low,nodo,padre);
        */
    }
    *nodo = new Nodo(low, high,val);
    (*nodo)->padre = padre;
    if(flag == -1) root->color = NEGRO;
    verificarMax(*nodo);
    Nodo *iter = *nodo;
    while(iter->padre){
        if(iter->padre->color == NEGRO)break;
        if(iter == root)break;
        if(iter->padre->color == ROJO){
            if(!iter->padre->padre->hijos[iter->padre->low < iter->padre->padre->low] or iter->padre->padre->hijos[iter->padre->low < iter->padre->padre->low]->color == NEGRO){
                if(iter->low > iter->padre->low){
                    if(iter->padre->low > iter->padre->padre->low) rotacionSimple(iter->padre->padre, DERECHA);
                    else rotacionCompleja(iter->padre->padre, IZQUIERDA);
                }
                else{
                    if(iter->padre->low > iter->padre->padre->low) rotacionCompleja(iter->padre->padre, DERECHA);
                    else rotacionSimple(iter->padre->padre, IZQUIERDA);
                }
                break;
            }
            else{
                iter->padre->padre->color = ROJO;
                iter->padre->padre->hijos[0]->color = NEGRO;
                iter->padre->padre->hijos[1]->color = NEGRO;
                iter = iter->padre->padre;
            }
        }
    }
    if(root->color == ROJO) root->color = NEGRO;
}

template <typename T, typename V>
void IntervalTree<T,V>::verificarMax(Nodo *nodo){
    if(!nodo)return;
    if(nodo->hijos[0]){
        if(nodo->hijos[1]){
            if(nodo->hijos[0]->high > nodo->hijos[1]->high and nodo->hijos[0]->high > nodo->high) nodo->max = nodo->hijos[0]->high;
            else if(nodo->hijos[1]->high > nodo->hijos[0]->high and nodo->hijos[1]->high > nodo->high) nodo->max = nodo->hijos[1]->high;
            else nodo->max = nodo->high;
        }
        else{
            if(nodo->hijos[0]->high > nodo->high) nodo->max = nodo->hijos[0]->high;
            else nodo->max = nodo->high;
        }
    }
    else{
        if(nodo->hijos[1]){
            if(nodo->hijos[1]->high > nodo->high) nodo->max = nodo->hijos[1]->high;
            else nodo->max = nodo->high;
        }
        else{
            nodo->max = nodo->high;
        }
    }
    verificarMax(nodo->padre);
}

template <typename T, typename V>
void IntervalTree<T,V>::rotacionCompleja(Nodo *& padre, bool flag){
    Nodo * hijo = padre->hijos[flag];
    Nodo * nieto = hijo->hijos[!flag];
    Nodo * abuelo = padre->padre;
    nieto->color = NEGRO;
    padre->color = ROJO;
    padre->hijos[flag] = nieto->hijos[!flag];
    if(nieto->hijos[!flag])
        nieto->hijos[!flag]->padre = padre;
    hijo->hijos[!flag] = nieto->hijos[flag];
    if(nieto->hijos[flag])
        nieto->hijos[flag]->padre = hijo;
    nieto->hijos[!flag] = padre;
    padre->padre = nieto;
    nieto->hijos[flag] = hijo;
    hijo->padre = nieto;
    if(padre == root){
        nieto->padre = nullptr;
        root = nieto;
    }
    else{
        if(abuelo) abuelo->hijos[abuelo->low < padre->low] = nieto;
        nieto->padre = abuelo;
    }
    if(padre->hijos[flag]){
        if(padre->hijos[flag]->high > padre->high) padre->max = padre->hijos[flag]->high;
        else padre->max = padre->high;
    }
    else{
        padre->max = padre->high;
    }
    if(hijo->hijos[!flag]){
        if(hijo->hijos[!flag]->high > padre->high) hijo->max = hijo->hijos[!flag]->high;
        else hijo->max = hijo->high;
    }
    else{
        hijo->max = hijo->high;
    }
    verificarMax(nieto);
}

template <typename T, typename V>
void IntervalTree<T,V>::rotacionSimple(Nodo *&padre, bool flag){
    Nodo * hijo = padre->hijos[flag];
    Nodo * abuelo = padre->padre;
    padre->hijos[flag] = hijo->hijos[!flag];
    if(padre->hijos[flag]){
        padre->hijos[flag]->padre = padre;
    }
    hijo->hijos[!flag] = padre;
    padre->padre = hijo;
    padre->color = ROJO;
    hijo->color = NEGRO;
    if(padre == root){
        hijo->padre = nullptr;
        root = hijo;
    }
    else{
        if(abuelo) abuelo->hijos[abuelo->low < padre->low] = hijo;
        hijo->padre = abuelo;
    }
    verificarMax(padre);
}

template <typename T, typename V>
int IntervalTree<T,V>::_find(T valor, Nodo **& nodo, Nodo *& padre){
    nodo = &(root);
    padre = nullptr;
    if(!root)return -1;
    while(*nodo){
        if((*nodo)->low == valor)return 1;
        if(!padre) padre = root;
        else padre = *nodo;
        nodo = &((*nodo)->hijos[(*nodo)->low < valor]);
    }
    return 0;
}

template <typename T, typename V>
bool IntervalTree<T,V>::__find(T valor, Nodo **& nodo, Nodo *& padre){
    nodo = &(root);
    padre = nullptr;
    if(!root)return false;
    while(*nodo){
        if(!padre) padre = root;
        else padre = *nodo;
        nodo = &((*nodo)->hijos[(*nodo)->low < valor]);
    }
    return true;
}

template <typename T, typename V>
IntervalTree<T,V>::Nodo::Nodo(T low, T high, V val){
    hijos[0] = nullptr;
    hijos[1] = nullptr;
    padre = nullptr;
    color = ROJO;
    this->valor = val;
    this->low = low;
    this->high = high;
}

template <typename T, typename V>
IntervalTree<T,V>::Nodo::Nodo(){
    hijos[0] = nullptr;
    hijos[1] = nullptr;
    padre = nullptr;
    color = ROJO;
}

template <typename T, typename V>
void IntervalTree<T,V>::Nodo::destruirme(){
    if(this->hijos[0])this->hijos[0]->destruirme();
    if(this->hijos[1])this->hijos[1]->destruirme();
    delete this;
}

template <typename T, typename V>
IntervalTree<T,V>::IntervalTree(){
    root = nullptr;
}

template <typename T, typename V>
IntervalTree<T,V>::~IntervalTree(){
    if(root) root->destruirme();
}

#endif // INTERVALTREE_H
