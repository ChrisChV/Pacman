#ifndef GAME_H
#define GAME_H
#include "Tablero.h"
#include "Fantasmita.h"
#include "Tempo.h"


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
    int n = 17;
    std::string * m = new std::string[n];
          //0123456789012345678901234567
    m[0] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    m[1] = "xbbbbbbbbbbbbxxxbbbbbbbbbbbbx";
    m[2] = "xbxxxxbxxxxxbxxxbxxxxbxxxxxbx";
    m[3] = "xBxxxxbxxxxxbxxxbxxxxbxxxxxBx";
    m[4] = "xbxxxxbxxxxxbxxxbxxxxbxxxxxbx";
    m[5] = "xbbbbbbbbbbbbbbbbbbbbbbbbbbbx";
    m[6] = "xbxxxxbxxxxxbxxxbxxxxbxxxxxbx";
    m[7] = "xbxxxxbxxxxxbxxxbxxxxbxxxxxbx";
    m[8] = "xbxxxxbbbbbbbbfbbbbbbbxxxxxbx";
    m[9] = "xbxxxxbxxxxxbxxxbxxxxbxxxxxbx";
    m[10] ="xbxxxxbxxxxxbxxxbxxxxbxxxxxbx";
    m[11] ="xbbbbbbbbbbbbbbbbbbbbbbbbbbbx";
    m[12] ="xbxxxxbxxxxxbxxxbxxxxbxxxxxbx";
    m[13] ="xBxxxxbxxxxxbxxxbxxxxbxxxxxBx";
    m[14] ="xbxxxxbxxxxxbxxxbxxxxbxxxxxbx";
    m[15] ="xbbbbbbbbbbbbbcbbbbbbbbbbbbbx";
    m[16] ="xxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

    //m[6] = "xbxxxxbxxbxxbxxbxxbxxbxxxxbx";
    //m[7] = "xbxxxxbxxbxxbxxbxxbxxbxxxxbx";
    //m[8] = "xbbbbbbxxbbbbbbbbbbxxbbbbbbx";
    //m[9] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    //m[11] ="xxxxxxbxx          xxbxxxxxx";
    //m[12] ="xxxxxxbxx xx xx xx xxbxxxxxx";







    /*


    //m[13] ="xxxxxxb   x      x   bxxxxxx";






   /*



    m[14] ="xxxxxxbxx xxxxxxxx xxbxxxxxx";
    m[15] ="xxxxxxbxx          xxbxxxxxx";
    m[16] ="xxxxxxbxx xxxxxxxx xxbxxxxxx";
    m[17] ="xbbbbbbbbbbbbxxbbbbbbbbbbbbx";
    m[18] ="xbxxxxbxxxxxbxxbxxxxxbxxxxbx";
    m[19] ="xbxxxxbxxxxxbxxbxxxxxbxxxxbx";
    m[20] ="xBbbxxbbbbbbbbbbbbbbbbxxbbBx";
    m[21] ="xxxbxxbxxbxxxxxxxxbxxbxxbxxx";
    m[22] ="xxxbxxbxxbxxxxxxxxbxxbxxbxxx";
    m[23] ="xbbbbbbxxbbbbxxbbbbxxbbbbbbx";
    m[24] ="xbxxxxxxxxxxbxxbxxxxxxxxxxbx";
    m[25] ="xbxxxxxxxxxxbxxbxxxxxxxxxxbx";
    m[26] ="xbbbbbbbbbbbbbbbbbbbbbbbbbbx";
    m[27] ="xxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    */


    allegro_init();
    install_keyboard();

    set_gfx_mode(GFX_AUTODETECT_WINDOWED, ANCHO_VENTANA, ALTO_VENTANA, 0, 0);

    BITMAP *buffer = create_bitmap(ANCHO_VENTANA,ALTO_VENTANA);

    clear_to_color(buffer, 0x999999);
    Tablero tab = Tablero(m,n,m[0].size(),"bloque.pcx",buffer);
    tab.dibujarTablero();
    tab.actualizarPosPacman();
    Tempo movimiento(400000);
    Tempo keyDown(100);
    tab.dibujarPacman();
    bool ganar = false;
    while(!key[KEY_ENTER] and !ganar){
        if(movimiento.actualizar()){
            tab.actualizarPacman();
            ganar = tab.ganar();
        }
        if(key[KEY_LEFT]) tab.actualizarDireccionPacman(DIR_IZQUIERDA);
        if(key[KEY_UP]) tab.actualizarDireccionPacman(DIR_ARRIBA);
        if(key[KEY_RIGHT]) tab.actualizarDireccionPacman(DIR_DERECHA);
        if(key[KEY_DOWN]) tab.actualizarDireccionPacman(DIR_ABAJO);
    }
}

#endif // GAME_H
