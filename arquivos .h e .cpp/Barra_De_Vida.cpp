#include "Barra_De_Vida.h"

Barra_De_Vida::Barra_De_Vida()
{

}
Barra_De_Vida::~Barra_De_Vida()
{
    if (icon != NULL)
        al_destroy_bitmap(icon);
    icon = NULL;
}
void Barra_De_Vida::inicializa()
{
    nJogador = ptrJ->getNumJogador();
    vidaI = vida = ptrJ->getVida();
    icon = al_load_bitmap("iconeHunter.png");
    if (!nJogador)
    {
        pos.getEspaco().setXY(10,10);
        corEnt = (al_map_rgba(255,255,255,255));
    }
    else
    {
        pos.getEspaco().setXY(610,10);
        corEnt = (al_map_rgba(0,255,255,255));
    }
}
void Barra_De_Vida::desenha()
{
    if (!ptrJ->getMorreu())
    {
        al_draw_tinted_scaled_bitmap(icon, corEnt, 0,0,11,13, pos.getEspaco().getX(), pos.getEspaco().getY(), 11*4, 13*4, 0);
        al_draw_filled_rectangle(pos.getEspaco().getX() + 44 + 4, pos.getEspaco().getY() + 52 - 20,
                                 pos.getEspaco().getX() + 44 + + 4 + 200*(vida/vidaI) ,pos.getEspaco().getY() + 52,
                                 al_map_rgb(0,0,0));
        if (vidaI != 0 && 200*(vida/vidaI) - 8 > 0)
        {
            al_draw_filled_rectangle(pos.getEspaco().getX() + 44 + 4 + 4, pos.getEspaco().getY() + 52 - 16,
                                 pos.getEspaco().getX() + 44 + + 4 - 4 + 200*(vida/vidaI) ,pos.getEspaco().getY() + 52 - 12,
                                 al_map_rgb(255,102,102));
        al_draw_filled_rectangle(pos.getEspaco().getX() + 44 + 4 + 4, pos.getEspaco().getY() + 52 - 12,
                                 pos.getEspaco().getX() + 44 + + 4 - 4 + 200*(vida/vidaI) ,pos.getEspaco().getY() + 52 - 8,
                                 al_map_rgb(255,0,0));
        al_draw_filled_rectangle(pos.getEspaco().getX() + 44 + 4 + 4, pos.getEspaco().getY() + 52 - 8,
                                 pos.getEspaco().getX() + 44 + + 4 - 4 + 200*(vida/vidaI) ,pos.getEspaco().getY() + 52 - 4,
                                 al_map_rgb(201,0,0));
        }
    }
}
void Barra_De_Vida::atualiza()
{
    if (!ptrJ->getMorreu())
        vida = ptrJ->getVida();
}
