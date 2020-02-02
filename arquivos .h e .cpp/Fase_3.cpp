#include "Fase_3.h"
#include "Barra_De_Vida.h"
#include "Fumaca.h"
#include "Jogo.h"
#include "Fantasma.h"

Fase_3::Fase_3()
{

}

Fase_3::~Fase_3()
{
    al_destroy_bitmap(dGlow);
    dGlow = NULL;
    al_destroy_bitmap(glow);
    glow = NULL;

    al_destroy_timer(timers[0]);
    if (timers != NULL)
        delete [] timers;
}
Fase_3::Fase_3(Lista_Entidades * listEnt, bool doisJ, Controle * control)
{
    listE = listEnt;
    listGEnt = listE->getListGen();
    doisJog = doisJ;
    pontoParada = 0;
    podePassar = true;
    invocou = false;
    terminouInvocar = false;
    audioTocando = false;
    chefaoMorto = false;
    areaChefao = false;
    progressao = 0;
    iteP1 = (inimigos.getTipoIni())[3].inicio();
    ang = 0;
    ptrBV = new Barra_De_Vida*[1 + doisJog];
    glow = al_load_bitmap("glow.png");
    dGlow = al_load_bitmap("darkGlow2.png");


    timers= new ALLEGRO_TIMER*[1];
    timers[0] = al_create_timer(1.0/60);

    string nome[] = {"F3", "S1-F3", "boss"};
    audioFase.inicializaAudio(nome, 3);

    ptrM = NULL;
    ptrJ[0] = NULL;
    ptrJ[1] = NULL;


    numPlat=38;
    numPlat += 4;
    inicializaFase(control);
}
void Fase_3::atualiza()
{
    alpha += fade;
    if (alpha >= 255 && fade == 1)
    {
        fade = 0;
        alpha = 255;
    }
    else if (alpha <= 0 && fade == -1)
    {
        fade = 0;
        alpha = 0;
    }

    if (doisJog && !ptrJ[1]->getMorreu())
    {
        if (!ptrJ[0]->getMorreu())
        {
            posX = (ptrJ[0]->getPos().getEspaco().getX() + ptrJ[1]->getPos().getEspaco().getX())/2;
        }
        else
        {
            posX = ptrJ[1]->getPos().getEspaco().getX();
        }
    }
    else
    {
        posX = ptrJ[0]->getPos().getEspaco().getX();
    }

    if (!audioTocando && posX <= 18000)
    {
        audioFase.tocaAudio("F3", 0.001, ALLEGRO_PLAYMODE_LOOP, 0);
        audioFase.fadeTo("F3", 0.01, 1);
        audioTocando = true;
    }
    if (posX > 18000)
    {
        if (audioFase.audioTocando("F3"))
            audioFase.fadeTo("F3", -0.005, 0);
        audioTocando = false;
    }
    if (posX <= 18000 && audioFase.audioTocando("boss"))
    {
        audioFase.fadeTo("boss", 0.01, 0);
    }
    Lista_Generica<Fumaca*>::Iterador iteFum;
    iteFum = listFum.inicio();
    for (int i = 0; i < NUM_FUMACA; i++, iteFum++)
    {
        if ((*iteFum)->getFuncionando())
        {
            (*iteFum)->atualiza();
        }
    }
    if (progressao == 0)
    {
        carregaInimigos(0);
        progressao++;
    }
    if (ptrM->getPos().getEspaco().getX() + 1200 >= 5600 && progressao == 1)
    {
        carregaInimigos(1);
        progressao++;
    }
    else if (ptrM->getPos().getEspaco().getX() + 1200 >= 14600 && progressao == 2)
    {
        carregaInimigos(2);
        progressao++;
    }
    if ((ptrM->getPos().getEspaco().getX() > 11700 || !podePassar) && pontoParada == 0)
    {
        pontoParada1();
    }
    if (posX >= 18800)
    {
        if (!areaChefao && alpha != 255)
        {

            fade = 1;
        }
        else if (!areaChefao&& alpha == 255)
        {
            if (!ptrJ[0]->getMorreu())
            {
                ptrJ[0]->setControleExterno(true);
                ptrJ[0]->getPos().getEspaco().setXY(19400,660 - ptrJ[0]->getBorda().getY());
                ptrJ[0]->getPos().getVelocidade().setXY(0,0);
                ptrJ[0]->getPos().getAceleracao().setXY(0,0);
            }
            if (doisJog && !ptrJ[1]->getMorreu())
            {
                ptrJ[1]->setControleExterno(true);
                ptrJ[0]->getPos().getEspaco().setXY(19300,660 - ptrJ[0]->getBorda().getY());
                ptrJ[1]->getPos().getVelocidade().setXY(0,0);
                ptrJ[1]->getPos().getAceleracao().setXY(0,0);
            }
            paraFase(18800, 20000);
            areaChefao = true;
        }
        else if (areaChefao && alpha != 0)
        {
            paraFase(18800, 20000);
            fade = -1;
            if (alpha == 1)
            {
                if (!ptrJ[0]->getMorreu())
                {
                    ptrJ[0]->setControleExterno(false);
                }
                if (doisJog && !ptrJ[1]->getMorreu())
                {
                    ptrJ[1]->setControleExterno(false);
                }
            }
        }
        else if (areaChefao && alpha == 0)
        {
            lutaChefao();
        }
        if (chefaoMorto && alpha != 255)
        {
            if (!ptrJ[0]->getMorreu())
                {
                    ptrJ[0]->setControleExterno(false);
                }
                if (doisJog && !ptrJ[1]->getMorreu())
                {
                    ptrJ[1]->setControleExterno(false);
                }
            paraFase(18800, 20000);
            fade = 1;
        }

    }

    if (ptrJ[0]->getMorreu() && (alpha == 255 || (doisJog && !ptrJ[1]->getMorreu())))
    {
        listE->mataJogador(0);
        ptrJ[0]->getPos().getEspaco().setXY(-1000, -1000);
        ptrJ[0]->setFuncionando(false);
    }
    if (doisJog && ptrJ[1]->getMorreu())
    {
        listE->mataJogador(1);
        ptrJ[1]->getPos().getEspaco().setXY(-1000, -1000);
        ptrJ[1]->setFuncionando(false);
    }
    if (doisJog)
    {
        if (ptrJ[0]->getMorreu() && ptrJ[1]->getMorreu() && alpha == 255)
        {
            if (!podePassar)
            {
                listIniPonto.limpar();
                al_stop_timer(timers[0]);
                al_set_timer_count(timers[0], 0);
                podePassar = true;
                invocou = false;
                terminouInvocar = false;
                if (pontoParada == 0)
                {
                    audioFase.fadeTo("S1-F3", -0.05, 0);
                }
                if (!audioFase.audioTocando("F3"))
                    audioFase.tocaAudio("F3", 0.01, ALLEGRO_PLAYMODE_LOOP, 0);

                audioFase.fadeTo("F3", 0.01, 1);

            }
            iteP1 = (inimigos.getTipoIni())[3].inicio();

            listE->resetaLista();
            listE->resetaJogador(ptrJ[0], 0);
            listE->resetaJogador(ptrJ[1], 1);
            listE->resetaScore();
            if (jogo->getCheckpoint() != 0)
                jogo->carregaPosicao(listE->getListGen());
            atualizaCheckpoint(jogo->getCheckpoint());
            areaChefao = false;
        }
        else if (ptrJ[0]->getMorreu() && ptrJ[1]->getMorreu() && alpha != 255)
        {
            fade = 1;
        }
    }
    else if (ptrJ[0]->getMorreu() && !doisJog && alpha ==255)
    {
        if (!podePassar)
        {
            listIniPonto.limpar();
            al_stop_timer(timers[0]);
            al_set_timer_count(timers[0], 0);

            invocou = false;
            terminouInvocar = false;
            if (pontoParada == 0)
            {
                audioFase.fadeTo("S1-F3", -0.05, 0);
            }
            if (!audioFase.audioTocando("F3"))
                audioFase.tocaAudio("F3", 0.01, ALLEGRO_PLAYMODE_LOOP, 0);

            audioFase.fadeTo("F3", 0.01, 1);
            podePassar = true;
        }
        iteP1 = (inimigos.getTipoIni())[3].inicio();

        listE->resetaLista();
        listE->resetaJogador(ptrJ[0], 0);
        listE->resetaScore();
        if (jogo->getCheckpoint() != 0)
            jogo->carregaPosicao(listE->getListGen());
        atualizaCheckpoint(jogo->getCheckpoint());
        areaChefao = false;
    }
    else if (ptrJ[0]->getMorreu() && !doisJog && alpha != 255)
    {
        fade = 1;
    }

    if (posX >= 5300 && jogo->getCheckpoint() <= 0)
    {
        jogo->setCheckpoint(1);
        jogo->salvaPosicao(listE->getListGen());
        listE->salvaScore();
    }
    else if (posX >= 17600 && jogo->getCheckpoint() <= 2)
    {
        jogo->setCheckpoint(3);
        jogo->salvaPosicao(listE->getListGen());
        listE->salvaScore();
    }
    else if (ptrJ[0]->getMorreu() && !doisJog && alpha != 255)
    {
        fade = 1;
    }
    ang++;
    atualizaEDano();
    ptrBV[0]->atualiza();
    if (doisJog)
        ptrBV[1]->atualiza();
    audioFase.atualizaAudio();
}
void Fase_3::desenha()
{
    Lista_Generica<Fumaca*>::Iterador iteFum;
    iteFum = listFum.inicio();
    for (int i = 0; i < NUM_FUMACA; i++, iteFum++)
    {
        if ((*iteFum)->getFuncionando())
        {
            (*iteFum)->desenha();
        }
    }

    al_draw_tinted_scaled_bitmap(dGlow, al_map_rgba(0,0,0,120 + 40*Math::coss[(ang/2)%360]), 0,0,948,666, 0,0,1200,720, 0);
    if (ptrJ[0] != NULL)
    {
        al_draw_tinted_scaled_bitmap(glow, al_map_rgba(27+ 5*Math::coss[(ang+45)%360],50+ 5*Math::coss[(ang+45)%360],50+ 10*Math::coss[(ang+45)%360], 5 + 5*Math::coss[(ang)%360]), 0,0, 500,500,
                                     (ptrJ[0])->getPos().getEspaco().getX() - ptrM->getPos().getEspaco().getX() - 250*((ptrJ[0])->getBorda().getY()/85.0),
                                     (ptrJ[0])->getPos().getEspaco().getY() - ptrM->getPos().getEspaco().getY() - 250*((ptrJ[0])->getBorda().getY()/85.0),
                                     ((ptrJ[0])->getBorda().getY()/85.0)*500, ((ptrJ[0])->getBorda().getY()/85.0)*500, 0);
    }
    if (ptrJ[1] != NULL)
    {
        al_draw_tinted_scaled_bitmap(glow, al_map_rgba(27+ 5*Math::coss[(ang+45)%360],50+ 5*Math::coss[(ang+45)%360],50+ 10*Math::coss[(ang+45)%360], 5 + 5*Math::coss[(ang)%360]), 0,0, 500,500,
                                     (ptrJ[1])->getPos().getEspaco().getX() - ptrM->getPos().getEspaco().getX() - 250*((ptrJ[1])->getBorda().getY()/85.0),
                                     (ptrJ[1])->getPos().getEspaco().getY() - ptrM->getPos().getEspaco().getY() - 250*((ptrJ[1])->getBorda().getY()/85.0),
                                     ((ptrJ[1])->getBorda().getY()/85.0)*500, ((ptrJ[1])->getBorda().getY()/85.0)*500, 0);
    }

    desenhaEDano();
    ptrBV[0]->desenha();
    if (doisJog)
        ptrBV[1]->desenha();

    al_draw_filled_rectangle(0,0,1200,720, al_map_rgba(0,0,0,alpha));

    if (posX < 18000 && alpha != 0)
    {

        if (doisJog)
        {
            if (!ptrJ[0]->getMorreu() || !ptrJ[0]->getMorreu())
                fade = -1;
        }
        else if (!ptrJ[0]->getMorreu())
            fade = -1;
    }
    if (chefaoMorto && alpha == 255)
    {
        passaFase(3);
    }

}
void Fase_3::inicializaFase(Controle * control)
{
    ptrM = new Mapa(10000, 420, NULL, numPlat,3);

    listE->incluiEntidade(static_cast<Entidade*>(ptrM));
    listE->setMapa(ptrM);

    ptrJ[0] = new Jogador(0);
    ptrJ[0]->setControle(control);
    ptrJ[0]->setListaEnt(listE);
    ptrJ[0]->setMapaAtual(ptrM);
    ptrM->setJogador(ptrJ[0], 0);

    ptrBV[0] = new Barra_De_Vida();
    ptrBV[0]->setPtrJ(ptrJ[0]);
    ptrBV[0]->inicializa();

    if (doisJog)
    {
        ptrJ[1] = new Jogador(1);
        ptrJ[1]->setControle(control);
        ptrJ[1]->setMapaAtual(ptrM);
        ptrJ[1]->setListaEnt(listE);
        ptrM->setJogador(ptrJ[1], 1);

        ptrBV[1] = new Barra_De_Vida();
        ptrBV[1]->setPtrJ(ptrJ[1]);
        ptrBV[1]->inicializa();
    }


    listE->incluiEntidade(static_cast<Entidade*>(ptrJ[0]));
    ptrJ[0]->inicializaEntidade();
    if (doisJog)
    {
        listE->incluiEntidade(static_cast<Entidade*>(ptrJ[1]));
        ptrJ[1]->inicializaEntidade();
    }

    inicializaInimigo();
    inicializaObstaculos();
    inicializaPlataformas();
    inicializaFumaca();
    inicializaEDano();

}
void Fase_3::inicializaInimigo()
{
    Inimigo * ptrI = NULL;
    for (int i = 0; i < NUM_SLIME_LAVA_3; i++)
    {
        ptrI=new Lava_Slime(-200,-200);
        ptrI->setJogador(ptrJ[0],0);
        ptrI->setListaEnt(listE);
        ptrI->setMapaAtual(ptrM);
        if (doisJog)
        {
            ptrI->setJogador(ptrJ[1],1);
        }
        listE->incluiEntidade(static_cast<Entidade*>(ptrI));
        listLavaSlime.inclui_frente(static_cast<Lava_Slime*>(ptrI));
        ptrI->inicializaEntidade();
    }
    for (int i = 0; i < NUM_FOGO_FATUO_3; i++)
    {
        ptrI=new Fogo_Fatuo(-200,-200);
        ptrI->setJogador(ptrJ[0],0);
        ptrI->setListaEnt(listE);
        ptrI->setMapaAtual(ptrM);
        if (doisJog)
        {
            ptrI->setJogador(ptrJ[1],1);
        }
        listE->incluiEntidade(static_cast<Entidade*>(ptrI));
        listFogoFatuo.inclui_frente(static_cast<Fogo_Fatuo*>(ptrI));
        ptrI->inicializaEntidade();
    }
    for (int i = 0; i < NUM_ESQUELETO_3; i++)
    {
        ptrI=new Esqueleto(-200,-200);
        ptrI->setJogador(ptrJ[0],0);
        ptrI->setListaEnt(listE);
        ptrI->setMapaAtual(ptrM);
        if (doisJog)
        {
            ptrI->setJogador(ptrJ[1],1);
        }
        listE->incluiEntidade(static_cast<Entidade*>(ptrI));
        listEsqueleto.inclui_frente(static_cast<Esqueleto*>(ptrI));
        ptrI->inicializaEntidade();
    }
    for (int i = 0; i < NUM_GOLEM_3; i++)
    {
        ptrI=new Golem(-200,-200);
        ptrI->setJogador(ptrJ[0],0);
        ptrI->setListaEnt(listE);
        ptrI->setMapaAtual(ptrM);
        if (doisJog)
        {
            ptrI->setJogador(ptrJ[1],1);
        }
        listE->incluiEntidade(static_cast<Entidade*>(ptrI));
        listGolem.inclui_frente(static_cast<Golem*>(ptrI));
        ptrI->inicializaEntidade();
    }
    for (int i = 0; i < NUM_GOLEM_MALIGNO_3; i++)
    {
        ptrI=new Golem_Maligno(-200,-200);
        ptrI->setJogador(ptrJ[0],0);
        ptrI->setListaEnt(listE);
        ptrI->setMapaAtual(ptrM);
        if (doisJog)
        {
            ptrI->setJogador(ptrJ[1],1);
        }
        listE->incluiEntidade(static_cast<Entidade*>(ptrI));
        listGolemMaligno.inclui_frente(static_cast<Golem_Maligno*>(ptrI));
        ptrI->inicializaEntidade();
    }
    for (int i = 0; i < NUM_NECROMANTE_3; i++)
    {
        ptrI=new Necromante(-200,-200);
        ptrI->setJogador(ptrJ[0],0);
        ptrI->setListaEnt(listE);
        ptrI->setMapaAtual(ptrM);
        if (doisJog)
        {
            ptrI->setJogador(ptrJ[1],1);
        }
        listE->incluiEntidade(static_cast<Entidade*>(ptrI));
        listNecromante.inclui_frente(static_cast<Necromante*>(ptrI));
        ptrI->inicializaEntidade();
    }
    ptrI = new Chefao(-200,-200);
    ptrI->setJogador(ptrJ[0],0);
    ptrI->setListaEnt(listE);
    ptrI->setMapaAtual(ptrM);
    if (doisJog)
    {
        ptrI->setJogador(ptrJ[1],1);
    }
    listE->incluiEntidade(static_cast<Entidade*>(ptrI));
    ptrChefao = static_cast<Chefao*>(ptrI);
    ptrI->inicializaEntidade();
}
void Fase_3::inicializaObstaculos()
{
    Obstaculo* ptrFan;
    for (int i = 0; i < NUM_FANTASMA; i++)
    {
        ptrFan = new Fantasma(-200, -200);
        ptrFan->setJogador(ptrJ[0],0);
        ptrFan->setMapaAtual(ptrM);
        {
            ptrFan->setJogador(ptrJ[1],1);
        }
        listE->incluiEntidade(static_cast<Entidade*>(ptrFan));
        listFantasma.inclui_frente(static_cast<Fantasma*>(ptrFan));
    }
    resetaObstaculos();
}
void Fase_3::resetaObstaculos()
{
    Lista_Generica<Fantasma*>::Iterador iteF;
    iteF = listFantasma.inicio();

    Lista_Generica<Vetor_R2> *posF;
    posF = inimigos.getPosFan();
    Lista_Generica<Vetor_R2>::Iterador iteposF;
    iteposF = posF[0].inicio();

    for (int i = 0; i < NUM_FANTASMA; i++, iteF++)
    {
        if ((*iteF)->getFuncionando() == true)
        {
            (*iteF)->setFuncionando(false);
            (*iteF)->resetaEntidade();
        }
    }
    iteF = listFantasma.inicio();
    for (int i = 0; i < NUM_FANTASMA; i++, iteposF++, iteF++)
    {
        (*iteF)->getPos().setEspaco((*iteposF));
        (*iteF)->setFuncionando(true);
    }
}
void Fase_3::inicializaPlataformas()
{
    tipoPlat=new int[numPlat];

    platX=new int*[numPlat];
    for(int i=0; i<numPlat; i++)
        platX[i]=new int[2];
    platY=new int*[numPlat];
    for(int i=0; i<numPlat; i++)
        platY[i]=new int[2];

    platX[0][0]=0;
    platY[0][0]=728;
    platX[0][1]=2144;
    platY[0][1]=840;
    tipoPlat[0]=0;

    platX[1][0]=2144;
    platY[1][0]=446;
    platX[1][1]=2924;
    platY[1][1]=840;
    tipoPlat[1]=0;

    platX[2][0]=2924;
    platY[2][0]=610;
    platX[2][1]=3260;
    platY[2][1]=840;
    tipoPlat[2]=0;

    platX[3][0]=4320;
    platY[3][0]=660;
    platX[3][1]=5100;
    platY[3][1]=840;
    tipoPlat[3]=0;

    platX[4][0]=5100;
    platY[4][0]=504;
    platX[4][1]=5430;
    platY[4][1]=840;
    tipoPlat[4]=0;

    platX[5][0]=5878;
    platY[5][0]=554;
    platX[5][1]=6214;
    platY[5][1]=840;
    tipoPlat[5]=0;

    platX[6][0]=6490;
    platY[6][0]=552;
    platX[6][1]=6830;
    platY[6][1]=840;
    tipoPlat[6]=0;

    platX[7][0]=7150;
    platY[7][0]=600;
    platX[7][1]=7488;
    platY[7][1]=840;
    tipoPlat[7]=0;

    platX[8][0]=7862;
    platY[8][0]=602;
    platX[8][1]=7976;
    platY[8][1]=840;
    tipoPlat[8]=0;

    platX[9][0]=8272;
    platY[9][0]=602;
    platX[9][1]=8386;
    platY[9][1]=840;
    tipoPlat[9]=0;

    platX[10][0]=8754;
    platY[10][0]=506;
    platX[10][1]=8868;
    platY[10][1]=840;
    tipoPlat[10]=0;

    platX[11][0]=9216;
    platY[11][0]=738;
    platX[11][1]=9334;
    platY[11][1]=840;
    tipoPlat[11]=0;

    platX[12][0]=9854;
    platY[12][0]=578;
    platX[12][1]=11200;
    platY[12][1]=596;
    tipoPlat[12]=1;

    platX[13][0]=11638;
    platY[13][0]=726;
    platX[13][1]=13366;
    platY[13][1]=840;
    tipoPlat[13]=0;

    platX[14][0]=11818;
    platY[14][0]=448;
    platX[14][1]=13164;
    platY[14][1]=466;
    tipoPlat[14]=1;

    platX[15][0]=14038;
    platY[15][0]=748;
    platX[15][1]=14302;
    platY[15][1]=840;
    tipoPlat[15]=0;

    platX[16][0]=14302;
    platY[16][0]=642;
    platX[16][1]=14536;
    platY[16][1]=840;
    tipoPlat[16]=0;

    platX[17][0]=14536;
    platY[17][0]=536;
    platX[17][1]=15916;
    platY[17][1]=840;
    tipoPlat[17]=0;

    platX[18][0]=15916;
    platY[18][0]=640;
    platX[18][1]=16168;
    platY[18][1]=840;
    tipoPlat[18]=0;

    platX[19][0]=16168;
    platY[19][0]=750;
    platX[19][1]=16432;
    platY[19][1]=840;
    tipoPlat[19]=0;

    platX[20][0]=17172;
    platY[20][0]=660;
    platX[20][1]=20000;
    platY[20][1]=840;
    tipoPlat[20]=0;


    platX[21][0]=platX[2][1];
    platY[21][0]=platY[2][0]-20;
    platX[21][1]=platX[2][1]+20;
    platY[21][1]=platY[2][0];
    tipoPlat[21]=3;



    platX[22][0]=platX[3][0]-20;
    platY[22][0]=platY[3][0]-20;
    platX[22][1]=platX[3][0];
    platY[22][1]=platY[3][0];
    tipoPlat[22]=3;


    platX[23][0]=platX[4][1];
    platY[23][0]=platY[4][0]-20;
    platX[23][1]=platX[4][1]+20;
    platY[23][1]=platY[4][0];
    tipoPlat[23]=3;

    platX[24][0]=platX[5][0]-20;
    platY[24][0]=platY[5][0]-20;
    platX[24][1]=platX[5][0];
    platY[24][1]=platY[5][0];
    tipoPlat[24]=3;

    platX[25][0]=platX[5][1];
    platY[25][0]=platY[5][0]-20;
    platX[25][1]=platX[5][1]+20;
    platY[25][1]=platY[5][0];
    tipoPlat[25]=3;

    platX[26][0]=platX[6][0]-20;
    platY[26][0]=platY[6][0]-20;
    platX[26][1]=platX[6][0];
    platY[26][1]=platY[6][0];
    tipoPlat[26]=3;

    platX[27][0]=platX[6][1];
    platY[27][0]=platY[6][0]-20;
    platX[27][1]=platX[6][1]+20;
    platY[27][1]=platY[6][0];
    tipoPlat[27]=3;

    platX[28][0]=platX[7][0]-20;
    platY[28][0]=platY[7][0]-20;
    platX[28][1]=platX[7][0];
    platY[28][1]=platY[7][0];
    tipoPlat[28]=3;

    platX[29][0]=platX[7][1];
    platY[29][0]=platY[7][0]-20;
    platX[29][1]=platX[7][1]+20;
    platY[29][1]=platY[7][0];
    tipoPlat[29]=3;

    platX[30][0]=platX[12][0]-20;
    platY[30][0]=platY[12][0]-20;
    platX[30][1]=platX[12][0];
    platY[30][1]=platY[12][0];
    tipoPlat[30]=3;

    platX[31][0]=platX[12][1];
    platY[31][0]=platY[12][0]-20;
    platX[31][1]=platX[12][1]+20;
    platY[31][1]=platY[12][0];
    tipoPlat[31]=3;

    platX[32][0]=platX[13][0]-20;
    platY[32][0]=platY[13][0]-20;
    platX[32][1]=platX[13][0];
    platY[32][1]=platY[13][0];
    tipoPlat[32]=3;

    platX[33][0]=platX[13][1];
    platY[33][0]=platY[13][0]-20;
    platX[33][1]=platX[13][1]+20;
    platY[33][1]=platY[13][0];
    tipoPlat[33]=3;

    platX[34][0]=platX[15][0]-20;
    platY[34][0]=platY[15][0]-20;
    platX[34][1]=platX[15][0];
    platY[34][1]=platY[15][0];
    tipoPlat[34]=3;

    platX[35][0]=platX[19][1];
    platY[35][0]=platY[19][0]-20;
    platX[35][1]=platX[19][1]+20;
    platY[35][1]=platY[19][0];
    tipoPlat[35]=3;

    platX[36][0]=platX[20][0]-20;
    platY[36][0]=platY[20][0]-20;
    platX[36][1]=platX[20][0];
    platY[36][1]=platY[20][0];
    tipoPlat[36]=3;

    platX[37][0]=19094;
    platY[37][0]=442;
    platX[37][1]=19882;
    platY[37][1]=460;
    tipoPlat[37]=1;

    for(int i = 0; i < numPlat - 4; i++)
    {
        for(int j = 0; j < 2; j++)
            ptrM->getPlat(i).getPos(j).setXY(platX[i][j], platY[i][j]);
        ptrM->getPlat(i).setTipo(tipoPlat[i]);
    }
}
void Fase_3::paraFase(float x1, float x2)
{
    if (ptrM->getPos().getEspaco().getX() < x1)
    {
        ptrM->getPos().getEspaco().setX(x1);
    }
    else if (ptrM->getPos().getEspaco().getX() > x2 - 1200)
    {
        ptrM->getPos().getEspaco().setX(x2 - 1200);
    }
}
void Fase_3::carregaInimigos(int t)
{
    Lista_Generica<Vetor_R2> *posI = inimigos.getPosIni();
    Lista_Generica<int> *tipoI =  inimigos.getTipoIni();
    Lista_Generica<Vetor_R2>::Iterador itePosI;
    itePosI = posI[t].inicio();
    Lista_Generica<int>::Iterador iteTipo;
    iteTipo = tipoI[t].inicio();

    Lista_Generica<Lava_Slime*>::Iterador iteLsl;
    iteLsl = listLavaSlime.inicio();
    int iLs = 0;
    Lista_Generica<Fogo_Fatuo*>::Iterador iteFog;
    iteFog = listFogoFatuo.inicio();
    int iF = 0;
    Lista_Generica<Esqueleto*>::Iterador iteEsq;
    iteEsq = listEsqueleto.inicio();
    int iE = 0;
    Lista_Generica<Golem*>::Iterador iteGol;
    iteGol = listGolem.inicio();
    int iG = 0;
    Lista_Generica<Golem_Maligno*>::Iterador iteGom;
    iteGom = listGolemMaligno.inicio();
    int iGm = 0;
    Lista_Generica<Necromante*>::Iterador iteNec;
    iteNec = listNecromante.inicio();
    int iN = 0;

    for (int i = 0; i <  tipoI[t].tamanho(); i++, itePosI++, iteTipo++)
    {
        switch (*iteTipo)
        {
        case SLIME_LAVA_3:
            for (; iLs < listLavaSlime.tamanho(); iLs++, iteLsl++)
            {
                if(!(*iteLsl)->getFuncionando())
                {
                    (*iteLsl)->getPos().setEspaco((*itePosI));
                    (*iteLsl)->setFuncionando(true);
                    iLs++;
                    iteLsl++;
                    break;
                }
            }
            break;
        case FOGO_FATUO_3:
            for (; iF < listFogoFatuo.tamanho(); iF++, iteFog++)
            {
                if(!(*iteFog)->getFuncionando())
                {
                    (*iteFog)->getPos().setEspaco((*itePosI));
                    (*iteFog)->setFuncionando(true);
                    iF++;
                    iteFog++;
                    break;
                }
            }
            break;
        case ESQUELETO_3:
            for (; iE < listEsqueleto.tamanho(); iE++, iteEsq++)
            {
                if(!(*iteEsq)->getFuncionando())
                {
                    (*iteEsq)->getPos().setEspaco((*itePosI));
                    (*iteEsq)->setFuncionando(true);
                    iE++;
                    iteEsq++;
                    break;
                }
            }
            break;
        case GOLEM_3:
            for (; iG < listGolem.tamanho(); iG++, iteGol++)
            {
                if(!(*iteGol)->getFuncionando())
                {
                    (*iteGol)->getPos().setEspaco((*itePosI));
                    (*iteGol)->setFuncionando(true);
                    iG++;
                    iteGol++;
                    break;
                }
            }
            break;
        case GOLEM_MALIGNO_3:
            for (; iGm < listGolemMaligno.tamanho(); iGm++, iteGom++)
            {
                if(!(*iteGom)->getFuncionando())
                {
                    (*iteGom)->getPos().setEspaco((*itePosI));
                    (*iteGom)->setFuncionando(true);
                    iGm++;
                    iteGom++;
                    break;
                }
            }
            break;
        case NECROMANTE_3:
            for (; iN < listNecromante.tamanho(); iN++, iteNec++)
            {
                if(!(*iteNec)->getFuncionando())
                {
                    (*iteNec)->getPos().setEspaco((*itePosI));
                    (*iteNec)->setFuncionando(true);
                    iN++;
                    iteNec++;
                    break;
                }
            }
            break;
        }
    }


}
void Fase_3::invocaInimigo(int t, Vetor_R2 p)
{
    Lista_Generica<Fogo_Fatuo*>::Iterador iteFog;
    iteFog = listFogoFatuo.inicio();
    Lista_Generica<Esqueleto*>::Iterador iteEsq;
    iteEsq = listEsqueleto.inicio();
    Lista_Generica<Golem*>::Iterador iteGol;
    iteGol = listGolem.inicio();
    Lista_Generica<Golem_Maligno*>::Iterador iteGom;
    iteGom = listGolemMaligno.inicio();
    Lista_Generica<Necromante*>::Iterador iteNec;
    iteNec = listNecromante.inicio();

    switch (t)
    {
    case FOGO_FATUO_3:
        for (int i = 0; i < listFogoFatuo.tamanho(); i++, iteFog++)
        {
            if (!(*iteFog)->getFuncionando())
            {
                (*iteFog)->getPos().setEspaco(p);
                (*iteFog)->setFuncionando(true);
                listIniPonto.inclui_frente(static_cast<Inimigo*>(*iteFog));
                break;
            }
        }
        break;
    case ESQUELETO_3:
        for (int i = 0 ; i < listEsqueleto.tamanho(); i++, iteEsq++)
        {
            if(!(*iteEsq)->getFuncionando())
            {
                (*iteEsq)->getPos().setEspaco(p);
                (*iteEsq)->setFuncionando(true);

                listIniPonto.inclui_frente(static_cast<Inimigo*>(*iteEsq));

                Lista_Generica<Fumaca*>::Iterador iteFum;
                iteFum = listFum.inicio();

                for (int i = 0; i < NUM_FUMACA; i++, iteFum++)
                {
                    if (!(*iteFum)->getFuncionando())
                    {
                        (*iteFum)->getPos().setEspaco((*iteEsq)->getPos().getEspaco() + Vetor_R2::vetor(-75,-70));
                        (*iteFum)->setAreaEfetiva((*iteEsq)->getBorda());
                        (*iteFum)->setFuncionando(true);
                        break;
                    }
                }
                break;
            }
        }
        break;
    case GOLEM_3:
        for (int i = 0 ; i < listGolem.tamanho(); i++, iteGol++)
        {
            if(!(*iteGol)->getFuncionando())
            {
                (*iteGol)->getPos().setEspaco(p);
                (*iteGol)->setFuncionando(true);

                listIniPonto.inclui_frente(static_cast<Inimigo*>(*iteGol));

                Lista_Generica<Fumaca*>::Iterador iteFum;
                iteFum = listFum.inicio();
                for (int i = 0; i < NUM_FUMACA; i++, iteFum++)
                {
                    if (!(*iteFum)->getFuncionando())
                    {
                        (*iteFum)->getPos().setEspaco((*iteGol)->getPos().getEspaco() + Vetor_R2::vetor(-210,-270));
                        (*iteFum)->setAreaEfetiva((*iteGol)->getBorda());
                        (*iteFum)->setFuncionando(true);
                        break;
                    }
                }
                break;
            }
        }
        break;
    case GOLEM_MALIGNO_3:
        for (int i = 0 ; i < listGolemMaligno.tamanho(); i++, iteGom++)
        {
            if(!(*iteGom)->getFuncionando())
            {
                (*iteGom)->getPos().setEspaco(p);
                (*iteGom)->setFuncionando(true);

                listIniPonto.inclui_frente(static_cast<Inimigo*>(*iteGom));

                Lista_Generica<Fumaca*>::Iterador iteFum;
                iteFum = listFum.inicio();
                for (int i = 0; i < NUM_FUMACA; i++, iteFum++)
                {
                    if (!(*iteFum)->getFuncionando())
                    {
                        (*iteFum)->getPos().setEspaco((*iteGom)->getPos().getEspaco() + Vetor_R2::vetor(-210,-270));
                        (*iteFum)->setAreaEfetiva((*iteGom)->getBorda());
                        (*iteFum)->setFuncionando(true);
                        break;
                    }
                }
                break;
            }
        }
        break;
    case NECROMANTE_3:
        for (int i = 0; i < listNecromante.tamanho(); i++, iteNec++)
        {
            if (!(*iteNec)->getFuncionando())
            {
                (*iteNec)->getPos().setEspaco(p);
                (*iteNec)->setFuncionando(true);
                listIniPonto.inclui_frente(static_cast<Inimigo*>(*iteNec));
                break;
            }
        }
        break;
    }
}
void Fase_3::pontoParada1()
{
    paraFase(11600, 13400);
    if (podePassar)
    {
        podePassar = false;
        al_set_timer_count(timers[0], 0);
        al_start_timer(timers[0]);
        audioFase.fadeTo("F3", -0.01, 0.001);
        audioFase.tocaAudio("S1-F3", 0.001, ALLEGRO_PLAYMODE_LOOP, 0);
        audioFase.fadeTo("S1-F3", 0.0016, 1);
    }
    if (al_get_timer_count(timers[0]) == 10*60 && !invocou)
    {

        for (int i = 0; i < 15; i++, iteP1++)
        {
            if ((*iteP1) == ESQUELETO_3 || (*iteP1) == GOLEM_3 || (*iteP1) == GOLEM_MALIGNO_3)
            {
                if (rand()%2)
                    invocaInimigo((*iteP1), Vetor_R2::vetor(11800 + 1350*(rand()%2), 650));
                else
                    invocaInimigo((*iteP1), Vetor_R2::vetor(11900 + 1200*(rand()%2), 400));
            }
            else if ((*iteP1)== FOGO_FATUO_3 || (*iteP1) == NECROMANTE_3)
                invocaInimigo((*iteP1), Vetor_R2::vetor(11650 + rand()%1700, 20));
        }
        invocou = true;
    }
    if (al_get_timer_count(timers[0]) > 10*60 && al_get_timer_count(timers[0]) <= 55*60 && (al_get_timer_count(timers[0])-10*60)%180 == 0)
    {
        if ((*iteP1) == ESQUELETO_3 || (*iteP1) == GOLEM_3 || (*iteP1) == GOLEM_MALIGNO_3)
        {
            if (rand()%2)
                invocaInimigo((*iteP1), Vetor_R2::vetor(11800 + 1350*(rand()%2), 650));
            else
                invocaInimigo((*iteP1), Vetor_R2::vetor(11900 + 1200*(rand()%2), 400));
        }
        else if ((*iteP1)== FOGO_FATUO_3 || (*iteP1) == NECROMANTE_3)
            invocaInimigo((*iteP1), Vetor_R2::vetor(11650 + rand()%1700, 20));
        iteP1++;
    }
    else if (al_get_timer_count(timers[0]) > 55*60 && al_get_timer_count(timers[0]) <= 105*60 && (al_get_timer_count(timers[0])-55*60)%120 == 0)
    {
        if ((*iteP1) == ESQUELETO_3 || (*iteP1) == GOLEM_3 || (*iteP1) == GOLEM_MALIGNO_3)
        {
            if (rand()%2)
                invocaInimigo((*iteP1), Vetor_R2::vetor(11800 + 1350*(rand()%2), 650));
            else
                invocaInimigo((*iteP1), Vetor_R2::vetor(11900 + 1200*(rand()%2), 400));
        }
        else if ((*iteP1)== FOGO_FATUO_3 || (*iteP1) == NECROMANTE_3)
            invocaInimigo((*iteP1), Vetor_R2::vetor(11650 + rand()%1700, 20));
        iteP1++;
    }
    else if (al_get_timer_count(timers[0]) > 105*60)
    {
        terminouInvocar = true;
    }
    if (terminouInvocar)
    {
        int i;
        Lista_Generica<Inimigo*>::Iterador iteI;
        iteI = listIniPonto.inicio();
        for (i = 0; i < listIniPonto.tamanho(); i++, iteI++)
        {
            if ((*iteI)->getFuncionando() &&
                    (*iteI)->getPos().getEspaco().getX() < 13400 &&
                    (*iteI)->getPos().getEspaco().getX() > 11600  &&
                    (*iteI)->getPos().getEspaco().getY() >= 0 &&
                    (*iteI)->getPos().getEspaco().getY() <= 840)
                break;
        }

        if (i == listIniPonto.tamanho())
        {
            al_stop_timer(timers[0]);
            al_set_timer_count(timers[0], 0);
            pontoParada++;
            podePassar = true;
            invocou = false;
            terminouInvocar = false;
            audioFase.fadeTo("F3", 0.01, 1);
            audioFase.fadeTo("S1-F3", -0.01, 0);

            jogo->setCheckpoint(2);
            jogo->salvaPosicao(listE->getListGen());
            listE->salvaScore();
        }
    }
}
void Fase_3::atualizaCheckpoint(int c)
{
    if (c == 0)
    {
        ptrJ[0]->getPos().getEspaco().setXY(100,560);
        if (!ptrJ[0]->getFuncionando())
        {
            ptrJ[0]->resetaEntidade();
            ptrJ[0]->setFuncionando(true);
        }
        if (doisJog)
        {
            ptrJ[1]->getPos().getEspaco().setXY(150,560);
            if (!ptrJ[1]->getFuncionando())
            {
                ptrJ[1]->resetaEntidade();
                ptrJ[1]->setFuncionando(true);
            }
        }
        ptrM->getPos().getEspaco().setX(0);
        progressao = 0;
        pontoParada = 0;
    }
    else if (c == 1)
    {
        ptrJ[0]->getPos().getEspaco().setXY(5300,400);
        if (!ptrJ[0]->getFuncionando())
        {
            ptrJ[0]->resetaEntidade();
            ptrJ[0]->setFuncionando(true);
        }
        if (doisJog)
        {
            ptrJ[1]->getPos().getEspaco().setXY(5300,400);
            if (!ptrJ[1]->getFuncionando())
            {
                ptrJ[1]->resetaEntidade();
                ptrJ[1]->setFuncionando(true);
            }
        }
        ptrM->getPos().getEspaco().setX(5300 - 600);
        progressao = 2;
        pontoParada = 0;
    }
    else if (c == 2)
    {
        ptrJ[0]->getPos().getEspaco().setXY(12400, 660);
        if (!ptrJ[0]->getFuncionando())
        {
            ptrJ[0]->resetaEntidade();
            ptrJ[0]->setFuncionando(true);
        }
        if (doisJog)
        {
            ptrJ[1]->getPos().getEspaco().setXY(12400, 660);
            if (!ptrJ[1]->getFuncionando())
            {
                ptrJ[1]->resetaEntidade();
                ptrJ[1]->setFuncionando(true);
            }
        }
        ptrM->getPos().getEspaco().setX(12400 - 660);
        progressao = 2;
        pontoParada = 1;
    }
    else if (c == 3)
    {
        ptrJ[0]->getPos().getEspaco().setXY(17600, 560);
        if (!ptrJ[0]->getFuncionando())
        {
            ptrJ[0]->resetaEntidade();
            ptrJ[0]->setFuncionando(true);
        }
        if (doisJog)
        {
            ptrJ[1]->getPos().getEspaco().setXY(17600, 560);
            if (!ptrJ[1]->getFuncionando())
            {
                ptrJ[1]->resetaEntidade();
                ptrJ[1]->setFuncionando(true);
            }
        }
        ptrM->getPos().getEspaco().setX(17000);
        progressao = 3;
        pontoParada = 1;
    }
    chefaoInvocado = false;
    areaChefao = false;
    resetaObstaculos();
}
void Fase_3::lutaChefao()
{
    if (!chefaoInvocado)
    {
        ptrChefao->getPos().getEspaco().setXY(19400, 20);
        ptrChefao->setFuncionando(true);
        audioFase.tocaAudio("boss", 0.001, ALLEGRO_PLAYMODE_LOOP, 0);
        audioFase.fadeTo("boss", 0.005, 1);
        chefaoInvocado = true;
    }
    paraFase(18800, 20000);
    ptrM->getPlat(ptrM->getNumPlat()-3).getPos(0).setXY(20000, -1000);
    ptrM->getPlat(ptrM->getNumPlat()-3).getPos(1).setXY(20150, 1000);
    ptrM->getPlat(ptrM->getNumPlat()-2).getPos(0).setXY(18750, -1000);
    ptrM->getPlat(ptrM->getNumPlat()-2).getPos(1).setXY(18800, 1000);
    ptrM->getPlat(ptrM->getNumPlat()-1).getPos(0).setXY(20000, -1000);
    ptrM->getPlat(ptrM->getNumPlat()-1).getPos(1).setXY(20150, 1000);

    if (ptrChefao->getFuncionando() == false)
    {
        chefaoMorto = true;
    }

}
