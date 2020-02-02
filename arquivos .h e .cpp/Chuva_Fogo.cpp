#include "Chuva_Fogo.h"

Chuva_Fogo::Chuva_Fogo()
{

}
Chuva_Fogo::Chuva_Fogo(Vetor_R2 posI) :
    Projetil(14, 62, "Chuva_Chefao.png", 2)
{
    /**PARAMETROS DO PROJETIL**/
    vidaBase = 1000000;
    vidaRand = 0;
    danoBase = 1000;
    danoRand = 100;
    /**PARAMETROS DO PROJETIL**/

    ID = 16;
    permiteColisaoEnt =true;
    permiteColisaoChao = true;
    borda.setXY(14, 62);
    tomandoDano=false;

    estado=0;
    desenhaFrame.setEstado(0);

    funcionando=false;


    corEnt = al_map_rgba(255,255,255,255);

    pos.setEspaco(posI);
    pos.getVelocidade().setXY(0,0);
    pos.getAceleracao().setXY(0,0);

    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;

    ptrJ = NULL;


    criaFrames();
    desenhaFrame.setEstado(0);
    desenhaFrame.atualizaFrame();

    desenhaFrame.setCoef(2);
    string nome[] = {"CHUVA_EXPLOSAO"};
    audioEnt.inicializaAudio(nome, 1);
}
Chuva_Fogo::~Chuva_Fogo()
{

}
void Chuva_Fogo::desenha()
{

    if(funcionando)
    {
        desenhaFrame.desenha(pos.getEspaco(), mapaAtual->getPos().getEspaco() + Vetor_R2::vetor(15,65), 0, false, corEnt);
    }


}
void Chuva_Fogo::atualiza()
{


    if(funcionando)
    {
        if(!estado && colisao[COLIDE_Y])
        {
            audioEnt.tocaAudio("CHUVA_EXPLOSAO", 1, ALLEGRO_PLAYMODE_ONCE, 0);
            estado=1;
            desenhaFrame.setEstado(1);
        }
        if(estado && (desenhaFrame.getFrameAtual()==estFrame[estado][1])&&
            desenhaFrame.getTempoEstado()>=tFrame[estado][desenhaFrame.getFrameAtual() - estFrame[estado][0]])
                resetaEntidade();

        desenhaFrame.atualizaFrame();
        if(!estado)
            pos.mover();
        audioEnt.atualizaAudio();

        if(vida<0)
        {
            resetaEntidade();
        }
    }


}
void Chuva_Fogo::criaFrames()
{
    int i;
    estFrame = desenhaFrame.getEstFrame();
    tFrame = desenhaFrame.getTFrame();

    estFrame[0][0] = 0;
    estFrame[0][1] = 5;
    estFrame[0][2] = 1;
    estFrame[1][0] = 6;
    estFrame[1][1] = 9;
    estFrame[1][2] = 0;

    for(i=0; i<6; i++)
        tFrame[0].inclui_frente(7);
    for(i=0; i<4; i++)
        tFrame[1].inclui_frente(4);
}
void Chuva_Fogo::setFuncionando(bool f)
{
    funcionando = f;
    pos.getAceleracao().setY(0.2);
}
void Chuva_Fogo::resetaEntidade()
{
    for (int i=0;i<3;i++)
        colisao[i]=false;
    funcionando=false;
    tomandoDano=false;
    estado=0;
    desenhaFrame.setEstado(0);
    desenhaFrame.atualizaFrame();

    pos.getEspaco().setXY(-200,-200);
    pos.getVelocidade().setXY(0,0);
    pos.getAceleracao().setXY(0,0);

    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;
}
