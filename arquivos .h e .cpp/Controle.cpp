#include "Controle.h"

Controle::Controle()
{
    for(int i = 0; i < NUM_KEYS; i++)
        teclas[i] = false;
    joystick[0] = NULL;
    joystick[1] = NULL;
}

Controle::~Controle()
{
}

bool Controle::recebeTeclasJogador(int n, bool t[])
{
    for(int i = NUM_TECLAS_JOGADOR*n; i < NUM_TECLAS_JOGADOR*(1 + n); i++)
        teclas[i] = t[i - NUM_TECLAS_JOGADOR*n];
}
void Controle::setTeclas(int i, bool t)
{
    teclas[i] = t;
}
bool Controle::getTeclas(int i)
{
    return teclas[i];
}
void Controle::atualizaControle()
{
    for(int i = 0; i < 2; i++)
    if (joystick[i] != NULL)
    {
        al_get_joystick_state(joystick[i], &joystickState);
        if (joystickState.stick[0].axis[0] < 0)
        {
            if (!botaoJoystick[LEFT + i*NUM_TECLAS_JOGADOR])
                teclas[LEFT + i*NUM_TECLAS_JOGADOR] = true;
            botaoJoystick[LEFT + i*NUM_TECLAS_JOGADOR] = true;
            botaoJoystick[RIGHT + i*NUM_TECLAS_JOGADOR] = false;
            teclas[RIGHT + i*NUM_TECLAS_JOGADOR] = false;
        }
        else if (joystickState.stick[0].axis[0] > 0)
        {
            if (!botaoJoystick[RIGHT + i*NUM_TECLAS_JOGADOR])
                teclas[RIGHT + i*NUM_TECLAS_JOGADOR] = true;
            botaoJoystick[RIGHT + i*NUM_TECLAS_JOGADOR] = true;
            teclas[LEFT + i*NUM_TECLAS_JOGADOR] = false;
            botaoJoystick[LEFT + i*NUM_TECLAS_JOGADOR] = false;
        }
        else
        {
            teclas[LEFT + i*NUM_TECLAS_JOGADOR] = false;
            botaoJoystick[LEFT + i*NUM_TECLAS_JOGADOR] = false;
            teclas[RIGHT + i*NUM_TECLAS_JOGADOR] = false;
            botaoJoystick[RIGHT + i*NUM_TECLAS_JOGADOR] = false;
        }

        if (joystickState.stick[0].axis[1] < -0.5)
        {
            if (!botaoJoystick[UP + i*NUM_TECLAS_JOGADOR])
                teclas[UP + i*NUM_TECLAS_JOGADOR] = true;
            botaoJoystick[UP + i*NUM_TECLAS_JOGADOR] = true;
            botaoJoystick[DOWN + i*NUM_TECLAS_JOGADOR] = false;
            teclas[DOWN + i*NUM_TECLAS_JOGADOR] = false;
        }
        else if (joystickState.stick[0].axis[1] > 0.5)
        {
            if (!botaoJoystick[DOWN + i*NUM_TECLAS_JOGADOR])
                teclas[DOWN + i*NUM_TECLAS_JOGADOR] = true;
            botaoJoystick[DOWN + i*NUM_TECLAS_JOGADOR] = true;
            teclas[UP + i*NUM_TECLAS_JOGADOR] = false;
            botaoJoystick[UP + i*NUM_TECLAS_JOGADOR] = false;
        }
        else
        {
            teclas[UP + i*NUM_TECLAS_JOGADOR] = false;
            botaoJoystick[UP + i*NUM_TECLAS_JOGADOR] = false;
            teclas[DOWN + i*NUM_TECLAS_JOGADOR] = false;
            botaoJoystick[DOWN + i*NUM_TECLAS_JOGADOR] = false;
        }
        if (joystickState.button[0])
        {
            if (!botaoJoystick[Z + i*NUM_TECLAS_JOGADOR])
                teclas[Z + i*NUM_TECLAS_JOGADOR] = true;
            botaoJoystick[Z + i*NUM_TECLAS_JOGADOR] = true;
        }
        else
        {
            teclas[Z + i*NUM_TECLAS_JOGADOR] = false;
            botaoJoystick[Z + i*NUM_TECLAS_JOGADOR] = false;
        }
        if (joystickState.button[1])
        {
            if (!botaoJoystick[X + i*NUM_TECLAS_JOGADOR])
                teclas[X + i*NUM_TECLAS_JOGADOR] = true;
            botaoJoystick[X + i*NUM_TECLAS_JOGADOR] = true;
        }
        else
        {
            teclas[X + i*NUM_TECLAS_JOGADOR] = false;
            botaoJoystick[X + i*NUM_TECLAS_JOGADOR] = false;
        }
        if (joystickState.button[2])
        {
            if (!botaoJoystick[C + i*NUM_TECLAS_JOGADOR])
                teclas[C + i*NUM_TECLAS_JOGADOR] = true;
            botaoJoystick[C + i*NUM_TECLAS_JOGADOR] = true;
        }
        else
        {
            teclas[C + i*NUM_TECLAS_JOGADOR] = false;
            botaoJoystick[C + i*NUM_TECLAS_JOGADOR] = false;
        }
        if (joystickState.button[9])
        {
            if (!botaoJoystick[V + i*NUM_TECLAS_JOGADOR])
                teclas[V + i*NUM_TECLAS_JOGADOR] = true;
            botaoJoystick[V + i*NUM_TECLAS_JOGADOR] = true;
        }
        else
        {
            teclas[V + i*NUM_TECLAS_JOGADOR] = false;
            botaoJoystick[V + i*NUM_TECLAS_JOGADOR] = false;
        }

    }
}
void Controle::reconfiguraControle()
{
    al_reconfigure_joysticks();
    if(al_get_num_joysticks() != 0)
    {
        joystick[0] = al_get_joystick(0);
        if (al_get_num_joysticks() == 2)
        {
            joystick[1] = al_get_joystick(1);
        }
        else
        {
            joystick[1] = NULL;
        }
    }
    else
    {
        joystick[0] = NULL;
        joystick[1] = NULL;
    }
}
void Controle::inicializaControle()
{
    al_reconfigure_joysticks();
    for (int i = 0; i < al_get_num_joysticks(); i++)
    {
        joystick[i] = al_get_joystick(i);
    }
}
