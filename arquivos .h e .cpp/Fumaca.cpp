#include "Fumaca.h"

Fumaca::Fumaca()
{
    ID = 32;
    permiteColisaoEnt =false;
    permiteColisaoChao = false;
    funcionando = false;
    colisao = new bool[NUM_COLISOES];
    for(int i = 0; i < NUM_COLISOES; i++)
        colisao[i] = false;
    dir = rand()%2;

    desenhaFrame.setNomeImg("Fumaca.png");
    desenhaFrame.setSpriteX(96);
    desenhaFrame.setSpriteY(96);
    desenhaFrame.setNumEstados(1);
    criaFrames();

    borda = Vetor_R2::vetor(0,0);
    areaEfetiva = Vetor_R2::vetor(0,0);
    pos.getEspaco().setXY(-200,-200);
    numFumaca = 0;
}

Fumaca::~Fumaca()
{

}
void Fumaca::desenha()
{
    desenhaFrame.desenha(pos.getEspaco(), mapaAtual->getPos().getEspaco() + Vetor_R2::vetor(63,67),
                         0, false, al_map_rgba(255,255,255,150) );
}
void Fumaca::atualiza()
{
    if (funcionando)
    {
        desenhaFrame.atualizaFrame();
        if (desenhaFrame.getFrameAtual() == 6 && desenhaFrame.getTempoEstado() == 3)
        {
            funcionando = false;
        }
    }
}
void Fumaca::setAreaEfetiva(Vetor_R2 a)
{
    areaEfetiva = a;
    if (a.getX() > a.getY())
    {
        desenhaFrame.setCoef(a.getX()/11);
    }
    else
    {
        desenhaFrame.setCoef(a.getY()/13);
    }
}
void Fumaca::setFuncionando(bool f)
{
    desenhaFrame.atualizaFrame();
    funcionando = f;
}
void Fumaca::criaFrames()
{
    estFrame = desenhaFrame.getEstFrame();
    tFrame = desenhaFrame.getTFrame();

    estFrame[0][0] = 0;
    estFrame[0][1] = 6;
    estFrame[0][2] = 1;

    for (int i = 0; i < 7; i++)
        tFrame[0].inclui_frente(4);
}
