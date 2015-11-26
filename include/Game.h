#ifndef GAME_H
#define GAME_H
#include "Tablero.h"
#include "Fantasmita.h"

int ANCHO_VENTANA = 1000;
int ALTO_VENTANA = 700;

class Game
{
    public:
        Game();
        virtual ~Game();
    protected:
    private:
};

Game::~Game(){

}

Game::Game(){
    int n = 11;
    std::string * m = new std::string[n];

    m[0] = "xxxxxxxxxxxxxxxxxxxxxxxxxx";
    m[1] = "x           xx           x";
    m[2] = "x xxxx xxxx xx xxxx xxxx x";
    m[3] = "x xxxx xxxx xx xxxx xxxx x";
    m[4] = "x xxxx xxxx xx xxxx xxxx x";
    m[5] = "x                        x";
    m[6] = "x xxxx xxxx xx xxxx xxxx x";
    m[7] = "x           xx           x";
    m[8] = "x xxxx xxxx    xxxx xxxx x";
    m[9] = "x           xx           x";
    m[10] = "xxxxxxxxxxxxxxxxxxxxxxxxxx";


    allegro_init();
    install_keyboard();

    set_gfx_mode(GFX_AUTODETECT_WINDOWED, ANCHO_VENTANA, ALTO_VENTANA, 0, 0);

    BITMAP *buffer = create_bitmap(ANCHO_VENTANA,ALTO_VENTANA);
    clear_to_color(buffer, 0x999999);
    Tablero tab = Tablero(m,n,m[0].size(),"bloque.pcx",buffer);
    tab.dibujarTablero();
    blit(buffer,screen,0,0,0,0,ANCHO_VENTANA,ALTO_VENTANA);
    readkey();
}

#endif // GAME_H
