#include "Efeito_Dano.h"

Efeito_Dano::Efeito_Dano()
{
    funcionando = false;
    pos.getEspaco().setXY(-200,-200);
    pos.getAceleracao().setY(0.05);
}
Efeito_Dano::~Efeito_Dano()
{

}
void Efeito_Dano::atualiza()
{
    if (funcionando)
    {
        pos.mover();
        alpha -= 0.05;
        if (pos.getVelocidade().getY() >= 0)
        {
            funcionando = false;
        }
    }


}
void Efeito_Dano::desenha()
{
    if (funcionando)
    {
        if (alpha > 0)
        {
            al_draw_textf(Entidade::font16, al_map_rgba(255*alpha,0,0,alpha*255), pos.getEspaco().getX() - mapaAtual->getPos().getEspaco().getX(),
                          pos.getEspaco().getY() - mapaAtual->getPos().getEspaco().getY(), ALLEGRO_ALIGN_CENTER,"%i", dano);
        }
        else
        {
            funcionando = false;
        }
    }

}
void Efeito_Dano::setDano(int d)
{
    dano = d;
    pos.getVelocidade().setY(-3);
    alpha = 1.0f;
}
