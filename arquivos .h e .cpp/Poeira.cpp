#include "Poeira.h"

Poeira::Poeira()
{

}
Poeira::Poeira(Vetor_R2 p, Vetor_R2 b, int t, bool rot, int d, bool desaparece, float vDes)
{
    tipo = t;
    rotaciona = rot;
    ID = 32;
    permiteColisaoEnt =false;
    permiteColisaoChao = false;
    funcionando = false;

    colisao = new bool[NUM_COLISOES];
    for(int i = 0; i < NUM_COLISOES; i++)
        colisao[i] = false;

    dir = d;
    des = desaparece;
    alpha = 1;
    vD = vDes;

    desenhaFrame.setNomeImg("poeira.png");
    desenhaFrame.setSpriteX(64);
    desenhaFrame.setSpriteY(64);
    borda = b;

    if (tipo == 1)
    {
        pos.setEspaco(p);

        pos.getEspaco() +=  Vetor_R2::vetor(borda.getX()*(dir*2 - 1) - 33, borda.getY() - 58);

        desenhaFrame.setNumEstados(1);
        estFrame = desenhaFrame.getEstFrame();
        tFrame = desenhaFrame.getTFrame();
        estFrame[0][0] = 0;
        estFrame[0][1] = 7;
        estFrame[0][2] = 0;
        tFrame[0].inclui_frente(3);
        tFrame[0].inclui_frente(4);
        tFrame[0].inclui_frente(4);
        tFrame[0].inclui_frente(5);
        tFrame[0].inclui_frente(6);
        tFrame[0].inclui_frente(6);
        tFrame[0].inclui_frente(6);
        desenhaFrame.setEstado(0);
    }
}

Poeira::~Poeira()
{

}
void Poeira::desenha()
{
    if (funcionando)
        desenhaFrame.desenha(pos.getEspaco(), mapaAtual->getPos().getEspaco(), 1 - dir, rotaciona, al_map_rgba(255*alpha,255*alpha,255*alpha,255*alpha));
}
void Poeira::atualiza()
{
    if (funcionando)
    {
        if (tipo == 1)
        {
            desenhaFrame.atualizaFrame();
            if (desenhaFrame.getFrameAtual() == 7)
            {
                alpha = 1;
                pos.getEspaco().setXY(-200,-200);
                funcionando = false;
            }
            if (des)
            {
                alpha -= vD;
                if (alpha <= 0.01)
                    alpha = 0.01;
            }

        }
    }

}
void Poeira::setFuncionando(bool f)
{
    desenhaFrame.setEstado(0);
    funcionando = f;
}
void Poeira::setDir(int d)
{
    dir = d;
}
void Poeira::setPos(Vetor_R2 p)
{
    pos.setEspaco(p);
    pos.getEspaco() +=  Vetor_R2::vetor(borda.getX()*(dir*2 - 1) - 33, borda.getY() - 58);
}
void Poeira::resetaEntidade()
{
    alpha = 1;
    pos.getEspaco().setXY(-200,-200);
    funcionando = false;
}
