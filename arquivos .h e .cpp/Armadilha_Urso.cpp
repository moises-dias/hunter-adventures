#include "Armadilha_Urso.h"

Armadilha_Urso::Armadilha_Urso()
{

}
Armadilha_Urso::Armadilha_Urso(int ix,int iy)
{
    pos.getEspaco().setXY(ix,iy);
    pos.getVelocidade().setXY(0, 0);
    pos.getAceleracao().setXY(0, 0);

    borda.setXY(75, 40);

    desenhaFrame.setSpriteX(52);
    desenhaFrame.setSpriteY(52);
    desenhaFrame.setNomeImg("Armadilha_Urso.png");

    desenhaFrame.setNumEstados(3);  /** 0=normal, 1=disparado, 2=fechado **/
    desenhaFrame.setGiraRot(false);


    /**PARAMETROS DO OBSTACULO**/
    vidaBase = 0;
    vidaRand = 0;
    danoBase = 800;
    danoRand = 80;
    /**PARAMETROS DO OBSTACULO**/

    vida = 10;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;


    cor=0;
    pisado=false;

    tomandoDano=false;
    funcionando=false;
    permiteColisaoEnt=false;

    construiFrames();

    desenhaFrame.setCoef(3.1);
    estado=0;
    desenhaFrame.setEstado(0);
    desenhaFrame.atualizaFrame();

    string nome[] = {"ARMADILHA_URSO"};
    audioEnt.inicializaAudio(nome, 1);
}
Armadilha_Urso::~Armadilha_Urso()
{

}

void Armadilha_Urso::desenha()
{
    if(funcionando)
    {
        if(cor>0)
            desenhaFrame.desenha(pos.getEspaco(), mapaAtual->getPos().getEspaco()+ Vetor_R2::vetor(80, 65),
                                  0, false, al_map_rgba(cor,cor,cor,cor));
    }
}
void Armadilha_Urso::atualiza()
{
    if(funcionando)
    {
        if(colisao[COLIDE_Y])
            pos.getAceleracao().setY(0);
        else
            pos.getAceleracao().setY(1);
        pos.mover();
        estado = desenhaFrame.getEstado();

        atualizaComportamento();
        atualizaEstados();

        desenhaFrame.setEstado(estado);
        desenhaFrame.atualizaFrame();

        audioEnt.atualizaAudio();
    }
}

void Armadilha_Urso::construiFrames()
{
    estFrame = desenhaFrame.getEstFrame();
    tFrame = desenhaFrame.getTFrame();

    estFrame[0][0]=0;
    estFrame[0][1]=0;
    estFrame[0][2]=1;
    estFrame[1][0]=1;
    estFrame[1][1]=1;
    estFrame[1][2]=1;
    estFrame[2][0]=2;
    estFrame[2][1]=2;
    estFrame[2][2]=1;

    tFrame[0].inclui_frente(100);
    tFrame[1].inclui_frente(100);
    tFrame[2].inclui_frente(100);
}
void Armadilha_Urso::atualizaComportamento()
{
    if(pisado && cor<255 && estado!=2)
    {
        cor+=3;
    }
    if(cor>255)
        cor=255;
    if(cor<0)
        cor=0;
    if(estado==2 && cor>=253)
    {
        permiteColisaoEnt=true;
        if(!audioEnt.audioTocando("ARMADILHA_URSO"))
            audioEnt.tocaAudio("ARMADILHA_URSO",1,ALLEGRO_PLAYMODE_ONCE,0);
    }
    else
        permiteColisaoEnt=false;
    if(estado==2)
        cor-=2;
}
void Armadilha_Urso::atualizaEstados()
{
    for(int i=0; i<2; i++)
    {
        if(jogadores[i]!=NULL && !pisado)
        {
            if(abs(jogadores[i]->getPos().getEspaco().getX()-pos.getEspaco().getX())<borda.getX() &&
                    (abs(jogadores[i]->getPos().getEspaco().getY()-pos.getEspaco().getY())<borda.getY()))
                pisado=true;
        }
    }
    if(estado==0 && cor>100)
        estado=1;
    if(estado==1 && cor==255)
        estado=2;
    if(estado==2 && cor==0)
        resetaEntidade();
}
void Armadilha_Urso::resetaEntidade()
{
    pos.getEspaco().setXY(-200,-200);
    pos.getVelocidade().setXY(0, 0);
    pos.getAceleracao().setXY(0, 0);

    pisado=false;
    funcionando=false;
    tomandoDano=false;

    vida=100;
    cor=0;
    permiteColisaoEnt=false;
    desenhaFrame.setGiraRot(false);

    estado=0;
    desenhaFrame.setEstado(0);
    desenhaFrame.atualizaFrame();
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;
}
void Armadilha_Urso::paraTimer()
{
}
