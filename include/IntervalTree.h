#ifndef INTERVALTREE_H
#define INTERVALTREE_H
#include "fstream"
#include "iostream"
#include "list"

enum Colores{NEGRO, ROJO};
enum Giros{IZQUIERDA, DERECHA};

template <typename T, typename V>
class IntervalTree
{
    public:
        class Nodo{
            public:
                Nodo();
                Nodo(T,T,T,V);
                T low;
                T high;
                T in;
                T max;
                V valor;
                Nodo * hijos[2];
                Nodo * padre;
                bool color;
                void destruirme();
        };
        IntervalTree();
        void insert(T,T,T,V);
        std::list<Nodo *> find(T,T);
        void print();
        void _print();
        int size(){return siz;};
        void verificarMax(Nodo *);
        Nodo * root;
        virtual ~IntervalTree();
    protected:
    private:

        int _find(T,T,T, Nodo **&, Nodo *&);
        bool __find(T, Nodo **&, Nodo *&);
        void rotacionSimple(Nodo *&, bool);
        void rotacionCompleja(Nodo *&, bool);
        int siz;
        void find(T,T,Nodo*, std::list<Nodo *> &res);

};

template <typename T, typename V>
void IntervalTree<T,V>::_print(){
    std::list<Nodo *> nodos;
    if(root) nodos.push_back(root);
    for(auto iter = nodos.begin(); iter != nodos.end(); iter++){
        std::cout<<"NODO->"<<(*iter)->low<<"--"<<(*iter)->high<<"--"<<(*iter)->max<<std::endl;
        if((*iter)->hijos[0]){
            nodos.push_back((*iter)->hijos[0]);
            std::cout<<"Izquierdo->"<<(*iter)->hijos[0]->low<<"--"<<(*iter)->hijos[0]->high<<std::endl;
        }
        if((*iter)->hijos[1]){
            nodos.push_back((*iter)->hijos[1]);
            std::cout<<"Derecho->"<<(*iter)->hijos[1]->low<<"--"<<(*iter)->hijos[1]->high<<std::endl;
        }
    }
}

template <typename T, typename V>
std::list<typename IntervalTree<T,V>::Nodo*> IntervalTree<T,V>::find(T low, T high){
    std::list<Nodo*> res;
    std::cout<<"LOW->"<<low<<std::endl;
    std::cout<<"HIGTH->"<<high<<std::endl;
    find(low,high,root,res);
    std::cout<<"AAAAAAAAAAA->"<<res.size()<<std::endl;
    return res;
}

template <typename T, typename V>
void IntervalTree<T,V>::find(T low, T high, Nodo *nodo, std::list<Nodo*>& res){
    if(!nodo) return;
    if(low >= nodo->low and high <= nodo->high){
        res.push_back(nodo);
        find(low,high,nodo->hijos[0],res);
        find(low,high,nodo->hijos[1],res);
    }
    else if(nodo->hijos[0] and nodo->hijos[0]->max > low){
        find(low,high,nodo->hijos[0],res);
    }
    else find(low,high,nodo->hijos[1],res);
}

template <typename T, typename V>
void IntervalTree<T,V>::print(){
    std::ofstream archivo("eje.dot");
    if(archivo.fail()){
        std::cout<<"EL archivo no se pudo abrir"<<std::endl;
        return;
    }
    archivo<<"digraph{"<<std::endl;
    std::list<Nodo *> result;
    if(root) result.push_back(root);
    while(!result.empty()){
        std::list<Nodo *> temp;
        for(auto iter = result.begin(); iter != result.end(); ++iter){
            std::string color;
            if((*iter)->color) color = "red";
            else color = "black";
            archivo<<(*iter)->low * (*iter)->high + 1<<" [label=\"["<<(*iter)->low<<","<<(*iter)->high<<"]"<<std::endl<<"max = "<<(*iter)->max<<"\", color = "<<color<<"];"<<std::endl;
            if((*iter)->hijos[0]){
                temp.push_back((*iter)->hijos[0]);
                archivo<<(*iter)->low * (*iter)->high + 1<<"->"<<(*iter)->hijos[0]->low * (*iter)->hijos[0]->high +1<<";"<<std::endl;
            }
            if((*iter)->hijos[1]){
                temp.push_back((*iter)->hijos[1]);
                archivo<<(*iter)->low * (*iter)->high + 1<<"->"<<(*iter)->hijos[1]->low * (*iter)->hijos[1]->high +1<<";"<<std::endl;
            }
        }
        result = temp;
    }
    archivo<<"}";
    archivo.close();
    system("dot -Tpdf eje.dot -o eje.pdf");
}

template <typename T, typename V>
void IntervalTree<T,V>::insert(T low, T high,T in, V val){
    Nodo ** nodo;
    Nodo * padre;
    int flag = this->_find(low,high,in, nodo, padre);
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
    *nodo = new Nodo(low, high,in,val);
    (*nodo)->padre = padre;
    if(flag == -1) root->color = NEGRO;
    Nodo *iter = *nodo;
    siz++;
    while(iter->padre){
        if(iter->padre->color == NEGRO)break;
        if(iter == root)break;
        if(iter->padre->color == ROJO){
            if(!iter->padre->padre->hijos[iter->padre->low <= iter->padre->padre->low] or iter->padre->padre->hijos[iter->padre->low < iter->padre->padre->low]->color == NEGRO){
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
    verificarMax(iter);
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
int IntervalTree<T,V>::_find(T valor,T valor2,T in, Nodo **& nodo, Nodo *& padre){
    nodo = &(root);
    padre = nullptr;
    if(!root)return -1;
    while(*nodo){
        if((*nodo)->low == valor and (*nodo)->high == valor2 and (*nodo)->in == in)return 1;
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
IntervalTree<T,V>::Nodo::Nodo(T low, T high,T in, V val){
    hijos[0] = nullptr;
    hijos[1] = nullptr;
    this->in = in;
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
    siz = 0;
}

template <typename T, typename V>
IntervalTree<T,V>::~IntervalTree(){
    if(root) root->destruirme();
}

#endif // INTERVALTREE_H
