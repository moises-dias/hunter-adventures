#include "Besouro.h"

Besouro::Besouro()
{

}
Besouro::Besouro(int ix,int iy)
{
    /**PARAMETROS DO INIMMIGO**/
    vidaBase = 2800;
    vidaRand = 280;
    danoBase = 800;
    danoRand = 80;
    score = 100;
    /**PARAMETROS DO INIMMIGO**/

    pos.getEspaco().setXY(ix,iy);
    pos.getVelocidade().setXY(0, 0);
    pos.getAceleracao().setXY(0, 0);
    pos.setPlatAtual(0);

    alvo=NULL;

    borda.setXY(20, 20);

    desenhaFrame.setSpriteX(52);
    desenhaFrame.setSpriteY(52);
    desenhaFrame.setNomeImg("Besouro.png");

    desenhaFrame.setNumEstados(4);

    permiteColisaoChao=true;

    colidePlat=false;

    funcionando=false;
    tomandoDano=false;
    atacou=false;

    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;
    dir=0;
    delX=0;
    delY=0;
    denominador=0;

    vRand = (rand()%1001)/1000.0;
    vXMax=2+vRand;
    velXDano=0;
    velYDano=0;

    construiFrames();

    corEnt = al_map_rgba(255,255,255,255);


    desenhaFrame.setCoef(2.1);
    desenhaFrame.setEstado(BES_VOANDO);
    desenhaFrame.atualizaFrame();


    string nome[] = {"BESOURO_MORTE", "BESOURO_DANO", "BESOURO_BZZ_LOOP"};
    audioEnt.inicializaAudio(nome, 3);

}
Besouro::~Besouro()
{

}
void Besouro::desenha()
{
    if(funcionando)
    {
        desenhaFrame.desenha(pos.getEspaco(), mapaAtual->getPos().getEspaco()+ Vetor_R2::vetor(55, 65), dir, false, corEnt);
    }
}
void Besouro::atualiza()
{
    if(funcionando)
    {
        if (!audioEnt.audioTocando("BESOURO_BZZ_LOOP") && estado!=BES_MORRENDO &&
            pos.getEspaco().getX()-mapaAtual->getPos().getEspaco().getX() > -50 &&
            pos.getEspaco().getX()-mapaAtual->getPos().getEspaco().getX() < 1250)
        {
            audioEnt.tocaAudio("BESOURO_BZZ_LOOP", 1, ALLEGRO_PLAYMODE_ONCE, 0);
        }


        estado = desenhaFrame.getEstado();

        atualizaComportamento();
        atualizaColisoes();
        atualizaEstados();

        pos.mover();

        if (tomandoDano)
            permiteColisaoEnt = false;
        else
            permiteColisaoEnt = true;

        if(vida <= 0 && estado!=BES_MORRENDO)
        {
            audioEnt.paraAudio("BESOURO_BZZ_LOOP");
            audioEnt.tocaAudio("BESOURO_MORTE", 1, ALLEGRO_PLAYMODE_ONCE, 0);
            estado=BES_MORRENDO;
            mudaScore(score);
            pos.getAceleracao().setXY(0,0);
        }


        desenhaFrame.setEstado(estado);
        desenhaFrame.atualizaFrame();

        audioEnt.atualizaAudio();
    }
}


void Besouro::atualizaComportamento()
{

    if(!(jogadores[0]==NULL&&jogadores[1]==NULL))
    {
        if(estado == BES_DANO)
        {
            if((abs(pos.getVelocidade().getX())>velXDano) || (abs(pos.getVelocidade().getY())>velYDano))
            {
                pos.getVelocidade().setX(pos.getVelocidade().getX()*0.95);
                pos.getVelocidade().setY(pos.getVelocidade().getY()*0.95);
            }
            else
            {
                pos.getVelocidade().setXY(0,0);
                pos.getAceleracao().setXY(0,0);
                estado = BES_VOANDO;
                tomandoDano=false;
            }


        }
        else if(estado == BES_PARADO)
        {
            pos.getAceleracao().setX(0);
            pos.getVelocidade().setX(0);
            pos.getAceleracao().setY(0);
            pos.getVelocidade().setY(0);
        }
        else if(estado == BES_MORRENDO)
        {
            if(!colisao[COLIDE_Y])
            {
                pos.getAceleracao().setY(0.5);/**ACELERACAO QUE ELE CAI ATE O CHAO**/
            }
            else
            {
                resetaEntidade();
            }

        }
        else
        {
            float x = pos.getEspaco().getX();
            float vx = pos.getVelocidade().getX();
            float ax = pos.getAceleracao().getX();

            if(jogadores[0]==NULL||jogadores[1]==NULL)
            {
                if(jogadores[0]==NULL)
                    alvo=jogadores[1];
                else
                    alvo=jogadores[0];
            }
            else
            {
                float p0x = jogadores[0]->getPos().getEspaco().getX();
                float p1x = jogadores[1]->getPos().getEspaco().getX();

                if(abs(x-p0x)<abs(x-p1x))
                    alvo=jogadores[0];
                else
                    alvo=jogadores[1];
            }

            delX=alvo->getPos().getEspaco().getX() - pos.getEspaco().getX();
            delY=alvo->getPos().getEspaco().getY() - pos.getEspaco().getY();
            if(delX>0)
                dir=0;
            else
                dir=1;

            if(abs(delX)>abs(delY))
                denominador=abs(delX);
            else
                denominador=abs(delY);


            if(denominador!=0)
            {
                pos.getVelocidade().setX(vXMax*delX/denominador);
                pos.getVelocidade().setY(vXMax*delY/denominador);
            }
            else
            {
                pos.getVelocidade().setX(0);
                pos.getVelocidade().setY(0);
            }
            if(abs(pos.getAceleracao().getX())>0.5)
            {
                if(pos.getAceleracao().getX()>0)
                    pos.getAceleracao().setX(pos.getAceleracao().getX()-0.2);
                else
                    pos.getAceleracao().setX(pos.getAceleracao().getX()+0.2);
            }
            else
                pos.getAceleracao().setX(0);
            if(abs(pos.getAceleracao().getY())>0.5)
            {
                if(pos.getAceleracao().getY()>0)
                    pos.getAceleracao().setY(pos.getAceleracao().getY()-0.2);
                else
                    pos.getAceleracao().setY(pos.getAceleracao().getY()+0.2);
            }
            else
                pos.getAceleracao().setY(0);

            float eX1, eBX1, eX2, eBX2;
            float eY1, eBY1, eY2, eBY2;
            eX1 = pos.getEspaco().getX();
            eBX1 = getBorda().getX();
            eX2 = alvo->getPos().getEspaco().getX();
            eBX2 = alvo->getBorda().getX();
            eY1 = pos.getEspaco().getY();
            eBY1 = getBorda().getY();
            eY2 = alvo->getPos().getEspaco().getY();
            eBY2 = alvo->getBorda().getY();
            if (eX1 + eBX1 >= eX2 - eBX2 &&
                    eX1 - eBX1 <= eX2 + eBX2 &&
                    eY1 + eBY1 >= eY2 - eBY2 &&
                    eY1 - eBY1 <= eY2 + eBY2)
            {
                pos.getAceleracao().setX(-2*pos.getVelocidade().getX());
                pos.getAceleracao().setY(-2*pos.getVelocidade().getY());
            }


        }
    }


}
void Besouro::atualizaColisoes()
{
    if (colisao[COLIDE_XD] && pos.getVelocidade().getX() >0)
    {
        pos.getVelocidade().setX(0);
    }
    if(colisao[COLIDE_XE] && pos.getVelocidade().getX() <0)
    {
        pos.getVelocidade().setX(0);
    }
}
void Besouro::atualizaEstados()
{
    if(tomandoDano && !(estado==BES_DANO) && !(estado==BES_MORRENDO))
    {
        audioEnt.tocaAudio("BESOURO_DANO", 1, ALLEGRO_PLAYMODE_ONCE, 0);
        estado = BES_DANO;
        pos.getVelocidade().setX((pos.getEspaco().getX()-posDano.getX())/5);
        pos.getVelocidade().setY((pos.getEspaco().getY()-posDano.getY())/5);

        if(posVelo.getX()>0)
            if(pos.getVelocidade().getX()<0)
                pos.getVelocidade().setX(-pos.getVelocidade().getX());

        if(posVelo.getX()<0)
            if(pos.getVelocidade().getX()>0)
                pos.getVelocidade().setX(-pos.getVelocidade().getX());

        if(abs(pos.getVelocidade().getX())<2)
        {
            if(pos.getVelocidade().getX()>0)
                pos.getVelocidade().setX(2);
            else if(pos.getVelocidade().getX()<0)
                pos.getVelocidade().setX(-2);
        }

        velXDano=abs(pos.getVelocidade().getX())/5;
        velYDano=abs(pos.getVelocidade().getX())/5;
        pos.getAceleracao().setXY(0,0);
    }
}
void Besouro::construiFrames()
{
    estFrame = desenhaFrame.getEstFrame();
    tFrame = desenhaFrame.getTFrame();

    estFrame[BES_VOANDO][0]=1;
    estFrame[BES_VOANDO][1]=2;
    estFrame[BES_VOANDO][2]=1;
    estFrame[BES_PARADO][0]=1;
    estFrame[BES_PARADO][1]=2;
    estFrame[BES_PARADO][2]=1;
    estFrame[BES_DANO][0]=0;
    estFrame[BES_DANO][1]=0;
    estFrame[BES_DANO][2]=1;
    estFrame[BES_MORRENDO][0]=3;
    estFrame[BES_MORRENDO][1]=3;
    estFrame[BES_MORRENDO][2]=1;

    tFrame[BES_VOANDO].inclui_frente(3);
    tFrame[BES_VOANDO].inclui_frente(3);

    tFrame[BES_PARADO].inclui_frente(3);
    tFrame[BES_PARADO].inclui_frente(3);

    tFrame[BES_DANO].inclui_frente(60);

    tFrame[BES_MORRENDO].inclui_frente(3);
}
void Besouro::criaEfeitos()
{

}

void Besouro::criaProjetil()
{

}

void Besouro::inicializaEntidade()
{

}
void Besouro::resetaEntidade()
{
    pos.getEspaco().setXY(-200,-200);
    pos.getVelocidade().setXY(0, 0);
    pos.getAceleracao().setXY(0, 0);

    funcionando=false;
    tomandoDano=false;
    atacou=false;

    alvo=NULL;

    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;

    estado=BES_VOANDO;
    desenhaFrame.setEstado(BES_VOANDO);
    desenhaFrame.atualizaFrame();
}
