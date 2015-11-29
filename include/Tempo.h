#ifndef TEMPO_H
#define TEMPO_H


class Tempo
{
    public:
        Tempo();
        Tempo(int time);
        void operator --();
        void operator ++();
        void reset();
        bool acabo();
        bool actualizar();
        virtual ~Tempo();
    protected:
    private:
        int time;
        int _time;
};

bool Tempo::actualizar(){
    if(acabo()){
        reset();
        return true;
    }
    time--;
    return false;;
}

void Tempo::reset(){
    time = _time;
}

bool Tempo::acabo(){
    if(time > 0)return false;
    return true;
}

void Tempo::operator++(){
    time++;
}

void Tempo::operator--(){
    time--;
}

Tempo::Tempo(int time){
    this->time = time;
    this->_time = time;
}

Tempo::Tempo(){
    time = 0;
    _time = 0;
}

Tempo::~Tempo(){

}

#endif // TEMPO_H
