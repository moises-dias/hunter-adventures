#ifndef CONTROLE_H
#define CONTROLE_H
#include "CEI.h"


class Controle
{
    public:
        Controle();
        ~Controle();

        bool recebeTeclasJogador(int n, bool t[]);
        void atualizaControle();
        void reconfiguraControle();
        void inicializaControle();
        void setTeclas(int i, bool t);
        bool getTeclas(int i);

    private:
        bool teclas[NUM_KEYS];
        bool botaoJoystick[NUM_KEYS];
        ALLEGRO_JOYSTICK * joystick[2];
        ALLEGRO_JOYSTICK_STATE joystickState;

};

#endif // CONTROLE_H
