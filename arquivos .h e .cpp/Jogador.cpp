#include "Jogador.h"
#include "Lista_Entidades.h"
#include "Poeira.h"
#include "Rastro.h"
#include "Ataque_Normal_Jogador.h"
#include "Ataque_Cima_Jogador.h"


void desabilitaTeclasAtaque(bool * teclas)
{
    teclas[C] = true;
    teclas[DOWN] = true;
    teclas[X] = true;
    teclas[Z] = true;
    teclas[RIGHT] = true;
    teclas[LEFT] = true;
}
void habilitaTeclasAtaque(bool * teclas)
{
    teclas[C] = false;
    teclas[DOWN] = false;
    teclas[X] = false;
    teclas[Z] = false;
    teclas[RIGHT] = false;
    teclas[LEFT] = false;
}
void habilitaTeclas(bool * teclas)
{
    for (int i = 0; i < NUM_TECLAS_JOGADOR; i++)
        teclas[i] = false;
}
bool estadosEstaticos(int est)
{
    if (est == ABAIXANDO)
        return true;
    else if (est== ABAIXADO)
        return true;
    else if (est == LEVANTANDO)
        return true;
    else if (est == VIRANDO_C)
        return true;
    return false;
}
bool estadosAtaque(int est)
{
    if (est == ATAQUE_A)
        return true;
    else if (est == ATAQUE_1)
        return true;
    else if (est == ATAQUE_2)
        return true;
    else if (est == ATAQUE_A_2)
        return true;
    else if (est == ATAQUE_C)
        return true;
    else if (est == ATAQUE_A_CI)
        return true;
    return false;
}
bool estadosAereos(int est)
{
    if (est == PULANDO)
        return true;
    else if (est == CAINDO)
        return true;
    return false;
}
Jogador::Jogador()
{

}
Jogador::Jogador(int nJ) :
    Entidade(52,52,"Hunter.png", NUM_ESTADOS)
{
    /**PARAMETROS DO JOGADOR**/
    vidaBase = 20000;
    vidaRand = 0;
    danoBase = 0;
    danoRand = 0;
    /**PARAMETROS DO JOGADOR**/


    ID=2;
    if (nJ == 0)
        pos.getEspaco().setX(300);
    else if (nJ == 1)
        pos.getEspaco().setX(400);
    numJogador = nJ;
    pos.getEspaco().setY(500);
    pos.getVelocidade().setXY(0, 0);
    pos.getAceleracao().setXY(0, 0);
    borda.setXY(14, 34);
    dir = 0;
    funcionando = true;
    permiteColisaoEnt = true;
    permiteColisaoChao = true;
    morreu = false;
    podeAT = true;
    combo = false;
    controleExterno = false;
    numAtaquesAereos = 0;
    alphaMorte = 255;
    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    dano = danoBase + rand()%(danoRand*2 + 1) - danoRand;
    for (int i = 0; i < NUM_KEYS; i++)
    {
        desabilitaTeclas[i] = false;
    }

    timerEnt = new ALLEGRO_TIMER* [7];
    timerEnt[0] = al_create_timer(1/20.0); /**Referente ao rastro da esquiva**/
    timerEnt[1] = al_create_timer(1/2.5); /**Referente ao tempo de pulo**/
    timerEnt[2] = al_create_timer(1/3.0);/**Referente ao tempo que fica na esquiva terrestre**/
    timerEnt[3] = al_create_timer(1.0);/**Referente ao tempo que pode se usar a esquiva denovo**/
    timerEnt[4] = al_create_timer(1/5.0); /**Referente ao rastro da corrida**/
    timerEnt[5] = al_create_timer(1/4.0); /**Referente ao tempo que fica na esquiva aerea**/
    timerEnt[6] = al_create_timer(1/2.0); /**Referente ao tempo que fica tomando dano**/

    teclas = new bool[NUM_TECLAS_JOGADOR];
    for(int i = 0; i < NUM_TECLAS_JOGADOR; i++)
        teclas[i] = false;
    colisao = new bool[NUM_COLISOES];
    for(int i = 0; i < NUM_COLISOES; i++)
        colisao[i] = false;



    corEnt = al_map_rgba(255*(1 - numJogador),255,255,255);
    desenhaFrame.setCoef(2);
    construiFrames();
    desenhaFrame.setEstado(PARADO);
    desenhaFrame.atualizaFrame();
    pos.setPlatAtual(0);

    string nome[] = {"puloHunter", "pisadaFloresta", "pisadaConcreto", "hunterEsquiva", "HunterAtaque1", "hunterMorrendo"};
    audioEnt.inicializaAudio(nome, 6);

    atNoJog[0] = NULL;
    atNoJog[1] = NULL;
    atNoJog[2] = NULL;

    al_start_timer(timerEnt[0]);
    al_start_timer(timerEnt[4]);



}

Jogador::~Jogador()
{
    for (int i = 0; i < 7; i++)
        if (timerEnt[i] != NULL)
            al_destroy_timer(timerEnt[i]);
    if (timerEnt != NULL)
        delete []timerEnt;
    timerEnt = NULL;
    if (teclas != NULL)
        delete[] teclas;
}
void Jogador::desenha()
{
    if (funcionando)
    {
        desenhaFrame.desenha(pos.getEspaco(), mapaAtual->getPos().getEspaco() + Vetor_R2::vetor(52, 70), dir, false, corEnt);
    }
}
void Jogador::atualiza()
{
    //Reseta o jogador para o estado do jogo atual
    if (funcionando)
    {
        for (int i = 0; i < NUM_TECLAS_JOGADOR; i ++)
            teclas[i] = contr->getTeclas(numJogador*NUM_TECLAS_JOGADOR + i);

            estado = desenhaFrame.getEstado();

            if (colisao[COLIDE_Y])/**Para o jogador em y se ele estiver colidindo com o chão ou uma plataforma**/
            {
                pos.getVelocidade().setY(0);
                pos.getAceleracao().setY(0);
            }
            else if (!estadosAtaque(estado))
                pos.getAceleracao().setY(1);

        if (!controleExterno)
        {
            movimentacao();
            esquiva();
            ataques();
            atualizaComportamento();
            atualizaColisoes();
            atualizaEstados();
        }
        criaEfeitos();
        criaProjetil();
        pos.mover();

        if(vida <= 0)
        {
            if (colisao[COLIDE_Y])
                estado = MORTE;
            else
                estado = DANO;
            permiteColisaoEnt = false;
            pos.getVelocidade().setXY(0,0);
            pos.getAceleracao().setXY(0,0);
            desabilitaTeclas[Z] = true;
            desabilitaTeclas[X] = true;
            desabilitaTeclas[C] = true;
            desabilitaTeclas[LEFT] = true;
            desabilitaTeclas[DOWN] = true;
            desabilitaTeclas[RIGHT] = true;
            desabilitaTeclas[UP] = true;
            if (!audioEnt.audioTocando("hunterMorrendo") && alphaMorte == 255)
                audioEnt.tocaAudio("hunterMorrendo", 0.8, ALLEGRO_PLAYMODE_ONCE, 0);
            alphaMorte -= 2;
            if (alphaMorte <= 0)
                alphaMorte = 0;
            corEnt = al_map_rgba(alphaMorte*(1 - numJogador),alphaMorte,alphaMorte,alphaMorte);
        }
        if (((estado == MORTE || estado == DANO) && alphaMorte == 0) || pos.getEspaco().getY() > 870)
        {
            morreu = true;
            estado = PARADO;
            pos.getVelocidade().setXY(0,0);
            pos.getAceleracao().setXY(0,0);
            for(int i = 0; i < NUM_COLISOES; i++)
                colisao[i] = false;
            for (int i = 0; i < 2; i++)
                atCiJog[i]->resetaEntidade();
            for (int i = 0; i < 3; i++)
                atNoJog[i]->resetaEntidade();
            for (int i = 0; i < 6; i++)
                rastJog[i]->resetaEntidade();
            for (int i = 0; i < 4; i++)
                poeJog[i]->resetaEntidade();
            desabilitaTeclas[Z] = false;
            desabilitaTeclas[X] = false;
            desabilitaTeclas[C] = false;
            desabilitaTeclas[LEFT] = false;
            desabilitaTeclas[DOWN] = false;
            desabilitaTeclas[RIGHT] = false;
            desabilitaTeclas[UP] = false;
        }


        desenhaFrame.setEstado(estado);
        desenhaFrame.atualizaFrame();

        for (int i = 0; i < NUM_TECLAS_JOGADOR; i ++)
            contr->setTeclas(numJogador*NUM_TECLAS_JOGADOR + i, teclas[i]);

    }
    audioEnt.atualizaAudio();

}
void Jogador::movimentacao()
{
    /**REFERENTES A MOVIMENTAÇÂO HORIZONTAL**/
    if (teclas[RIGHT] && !desabilitaTeclas[RIGHT])
    {
        if (pos.getVelocidade().getX() < 6)
        {
            pos.getAceleracao().setX(0.5);
        }
        else if (pos.getVelocidade().getX() >= 7)
        {
            pos.getAceleracao().setX(0);
            pos.getVelocidade().setX(7);
        }
        if ((estado == PARADO || (estado == PARANDO && dir == 0)) && estado != VIRANDO_C && !colisao[COLIDE_XD])
            mudaEstado(INI_CORRIDA);
        else if ((estado == CORRENDO || estado == PARANDO) && dir == 1)
        {
            mudaEstado(VIRANDO_C);
        }

        dir = 0;
    }
    if (teclas[LEFT] && !desabilitaTeclas[LEFT])
    {
        if (pos.getVelocidade().getX() > -6)
        {
            pos.getAceleracao().setX(-0.5);
        }
        else if (pos.getVelocidade().getX() <= -7)
        {
            pos.getAceleracao().setX(0);
            pos.getVelocidade().setX(-7);
        }
        if ((estado == PARADO || (estado == PARANDO && dir == 1)) && estado != VIRANDO_C && !colisao[COLIDE_XE])
            mudaEstado(INI_CORRIDA);
        else if ((estado == CORRENDO || estado == PARANDO) && dir == 0)
            mudaEstado(VIRANDO_C);
        dir = 1;
    }
    /**REFERENTES A MOVIMENTAÇÂO HORIZONTAL**/

    /**REFERENTES AO PULO E SE ABAIXAR**/
    if ((teclas[X] && !desabilitaTeclas[X]) && !teclas[DOWN])
    {
        if (!al_get_timer_started(timerEnt[1]) && colisao[COLIDE_Y])
            al_start_timer(timerEnt[1]);
        mudaEstado(PULANDO);
        if (!audioEnt.audioTocando("puloHunter") && colisao[COLIDE_Y])
            audioEnt.tocaAudio("puloHunter", 1, ALLEGRO_PLAYMODE_ONCE, 0);
        pos.setPlatAtual(0);
    }
    if ((teclas[X] && !desabilitaTeclas[X]) && al_get_timer_started(timerEnt[1]) && al_get_timer_count(timerEnt[1]) == 0)
    {
        pos.getVelocidade().setY(-14);
        colisao[COLIDE_Y] = false;
    }
    if (al_get_timer_started(timerEnt[1]) && al_get_timer_count(timerEnt[1]) == 0 && !teclas[X])
    {
        /**Impede o jogador de dar 2 pulos antes do timer terminar se ele ja deixou de apertar X**/
        desabilitaTeclas[X] = true;
        al_stop_timer(timerEnt[1]);
        al_set_timer_count(timerEnt[1], 0);
    }
    if (al_get_timer_started(timerEnt[1]) && al_get_timer_count(timerEnt[1]) != 0)
    {
        al_stop_timer(timerEnt[1]);
        al_set_timer_count(timerEnt[1], 0);
        desabilitaTeclas[X] = false;
        teclas[X] = false;
    }
    if ((teclas[DOWN] && !desabilitaTeclas[DOWN]) && estado != ABAIXADO && colisao[COLIDE_Y] && !estadosAtaque(estado))
    {
        mudaEstado(ABAIXANDO);
    }
    if ((teclas[DOWN] && !desabilitaTeclas[DOWN]) && (teclas[X] && !desabilitaTeclas[X]) && mapaAtual->getPlat(pos.getPlatAtual()).getTipo() == 1)
    {
        mudaEstado(CAINDO);
        if (pos.getVelocidade().getY() == 0)
            pos.getVelocidade().setY(5);
        colisao[COLIDE_Y] = false;
    }
    /**REFERENTES AO PULO E SE ABAIXAR**/
}
void Jogador::esquiva()
{
    /**REFERENTES A ESQUIVA NO CHAO**/

    if ((teclas[C] && !desabilitaTeclas[C]) && colisao[COLIDE_Y] && !al_get_timer_started(timerEnt[3]))
    {

        mudaEstado(C_ESQUIVA);
        dirEsq = dir;
    }

    if (estado == ESQUIVA && colisao[COLIDE_Y])
    {
        if (dirEsq== 0 && pos.getVelocidade().getX() < 14)
        {
            pos.getVelocidade().setX(14);

        }
        else if (dirEsq == 1 && pos.getVelocidade().getX() > -14)
        {
            pos.getVelocidade().setX(-14);
        }
        if (dirEsq != dir)
            dir = dirEsq;
    }
    if (al_get_timer_count(timerEnt[2]))
    {
        if (dirEsq == 0 && teclas[RIGHT])
            mudaEstado(CORRENDO);
        else if (dirEsq == 1 && teclas[LEFT])
            mudaEstado(CORRENDO);
        else
            mudaEstado(PARANDO);
    }
    /**REFERENTES A ESQUIVA NO CHAO**/

    /**REFERENTES A ESQUIVA NO AR**/
    if ((teclas[C] && !desabilitaTeclas[C]) && !colisao[COLIDE_Y] && !al_get_timer_started(timerEnt[3]))
    {
        mudaEstado(ESQUIVA);
        audioEnt.tocaAudio("hunterEsquiva", 1, ALLEGRO_PLAYMODE_ONCE, 0);
        dirEsq = dir;
    }

    if (estado == ESQUIVA && !colisao[COLIDE_Y])
    {
        pos.getAceleracao().setY(0);
        if (dirEsq== 0 && pos.getVelocidade().getX() < 10)
        {
            pos.getVelocidade().setXY(10,0);

        }
        else if (dirEsq == 1 && pos.getVelocidade().getX() > -10)
        {
            pos.getVelocidade().setXY(-10,0);
        }
        if (dirEsq != dir)
            dir = dirEsq;
    }
    if (al_get_timer_count(timerEnt[5]))
    {

        teclas[C] = false;
        if (dirEsq == 0 && teclas[RIGHT])
            mudaEstado(CORRENDO);
        else if (dirEsq == 1 && teclas[LEFT])
            mudaEstado(CORRENDO);
        else
            mudaEstado(PARANDO);
    }
    /**REFERENTES A ESQUIVA NO AR**/

    if (al_get_timer_count(timerEnt[3]))
    {
        al_stop_timer(timerEnt[3]);
        al_set_timer_count(timerEnt[3], 0);
    }
}
void Jogador::ataques()
{

    /**REFERENTES AOS ATAQUES TERRESTRES**/
    if ((teclas[Z] && !desabilitaTeclas[Z]) && colisao[COLIDE_Y])
    {

        if (teclas[UP])
            mudaEstado(ATAQUE_C);
        else
            mudaEstado(ATAQUE_1);

    }
    if (teclas[Z] && (estado == ATAQUE_1 || estado == ATAQUE_2))
    {
        combo = true;
    }
    /**REFERENTES AOS ATAQUES TERRESTRES**/


    /**REFERENTES AOS ATAQUES AÉREOS**/
    if (teclas[Z] && !colisao[COLIDE_Y] && !estadosAtaque(estado) && numAtaquesAereos < 3)
    {

        if (teclas[UP])
        {
            mudaEstado(ATAQUE_A_CI);
        }
        else
            mudaEstado(ATAQUE_A);
        numAtaquesAereos++;
    }
    if (teclas[Z] && (estado == ATAQUE_A || estado == ATAQUE_A_2) && numAtaquesAereos < 3)
    {
        combo = true;
    }
    /**REFERENTES AOS ATAQUES AÉREOS**/

    /**REFERENTES AO COMBO**/
    if (combo)
    {

        if (estado == ATAQUE_1 && ((desenhaFrame.getFrameAtual() == 19 && desenhaFrame.getTempoEstado() >= 5) || (desenhaFrame.getFrameAtual() == 20)))
        {
            mudaEstado(ATAQUE_2);

        }
        else if (estado == ATAQUE_2 && ((desenhaFrame.getFrameAtual() == 38 && desenhaFrame.getTempoEstado() >= 5) || (desenhaFrame.getFrameAtual() == 39)))
        {

            mudaEstado(ATAQUE_1);
            desenhaFrame.setEstado(estado);
            desenhaFrame.setFrameAtual(17);
        }
        else if (estado == ATAQUE_A && ((desenhaFrame.getFrameAtual() == 27 && desenhaFrame.getTempoEstado() >= 3) || (desenhaFrame.getFrameAtual() == 28)))
        {
            numAtaquesAereos++;
            mudaEstado(ATAQUE_A_2);
            desenhaFrame.setEstado(estado);
            desenhaFrame.setFrameAtual(42);
        }
        else if (estado == ATAQUE_A_2 && ((desenhaFrame.getFrameAtual() == 46 && desenhaFrame.getTempoEstado() >= 3) || (desenhaFrame.getFrameAtual() > 47)))
        {
            numAtaquesAereos++;

            mudaEstado(ATAQUE_A);
            desenhaFrame.setEstado(estado);
            desenhaFrame.setFrameAtual(25);
        }
        combo = false;
    }
    /**REFERENTES AO COMBO**/
}
void Jogador::atualizaComportamento()
{
    /**REFERENTES AOS COMPORTAMENTOS DO PERSONAGEM INVARIANTES DO CONTROLADOR**/
    /**INERCIA**/
    if (!teclas[RIGHT] && !teclas[LEFT] && pos.getAceleracao().getX() != 0)
    {
        pos.getAceleracao().setX(0);
    }
    if (!teclas[RIGHT] && !teclas[LEFT] && estado == CORRENDO)
    {
        mudaEstado(PARANDO);
    }
    if ((colisao[COLIDE_Y]||estadosAtaque(estado)) && estado != ESQUIVA && estado != CORRENDO &&
            (!teclas[RIGHT] || desabilitaTeclas[RIGHT])&& (!teclas[LEFT] || desabilitaTeclas[LEFT]))
    {
        if (pos.getVelocidade().getX() < 3 && pos.getVelocidade().getX() >-3)
        {
            pos.getVelocidade().setX(0);
            pos.getAceleracao().setX(0);
            if (estado == PARANDO)
                mudaEstado(PARADO);
        }
        else if (pos.getVelocidade().getX() >= 1)
        {
            if (estado== ATAQUE_A || estado== ATAQUE_A_2 || estado== ATAQUE_A_CI)
                pos.getAceleracao().setX(-0.15);
            else if (estado == ESQUIVA)
                pos.getAceleracao().setX(-0.5);
            else
                pos.getAceleracao().setX(-0.3);
            if (!estadosAtaque(estado) && !estadosEstaticos(estado))
                mudaEstado(PARANDO);
        }
        else if (pos.getVelocidade().getX() <= -1)
        {
            if (estado== ATAQUE_A || estado== ATAQUE_A_2 || estado== ATAQUE_A_CI)
                pos.getAceleracao().setX(0.15);
            else if (estado == ESQUIVA)
                pos.getAceleracao().setX(0.5);
            else
                pos.getAceleracao().setX(0.3);
            if (!estadosAtaque(estado) && !estadosEstaticos(estado))
                mudaEstado(PARANDO);
        }
    }
    /**INERCIA**/

    if (colisao[COLIDE_Y] && al_get_timer_started(timerEnt[1]))
    {
        al_stop_timer(timerEnt[1]);
    }
    if (estado == ATAQUE_A || estado == ATAQUE_A_2 || estado == ATAQUE_A_CI)
    {
        pos.getVelocidade().setY(0);
        pos.getAceleracao().setY(0);
    }
    if (colisao[COLIDE_Y])
    {
        numAtaquesAereos = 0;
    }
    if (estado == ESQUIVA)
        permiteColisaoEnt = false;
    else if (!al_get_timer_started(timerEnt[6]))
        permiteColisaoEnt = true;
    if (!teclas[DOWN] && estado == ABAIXADO)
    {
        mudaEstado(LEVANTANDO);
    }
    if ((estado == ATAQUE_1 || estado == ATAQUE_2 || estado == ATAQUE_C) && !colisao[COLIDE_Y])
    {
        mudaEstado(CAINDO);
    }
    if (tomandoDano)
    {
        mudaEstado(DANO);
    }
    if (al_get_timer_started(timerEnt[6]) && al_get_timer_count(timerEnt[6]) == 1 && estado == DANO)
    {
        mudaEstado(PARADO);
    }
    if (al_get_timer_started(timerEnt[6]) && al_get_timer_count(timerEnt[6]) == 4)
    {
        al_stop_timer(timerEnt[6]);
        al_set_timer_count(timerEnt[6], 0);
        permiteColisaoEnt = true;
    }

    /**REFERENTES AOS COMPORTAMENTOS DO PERSONAGEM INVARIANTES DO CONTROLADOR**/



}
void Jogador::atualizaColisoes()
{
    /**Para o jogador em x se ele estiver colidindo com o chão pela lateral**/
    if (colisao[COLIDE_XD] && pos.getVelocidade().getX() >0)
    {
        pos.getVelocidade().setX(0);
        if (estado == CORRENDO)
        {
            mudaEstado(PARADO);
        }
    }
    if(colisao[COLIDE_XE] && pos.getVelocidade().getX() <0)
    {
        pos.getVelocidade().setX(0);
        if (estado == CORRENDO)
        {
            mudaEstado(PARADO);
        }
    }
}
void Jogador::atualizaEstados()
{
    if ((estado == CAINDO || estado == PULANDO) && (pos.getVelocidade().getX() != 0) && colisao[COLIDE_Y])
    {
        mudaEstado(CORRENDO);
    }
    else if ((estado == CAINDO || estado == PULANDO) && (pos.getVelocidade().getX() == 0) && colisao[COLIDE_Y])
    {
        mudaEstado(PARADO);
    }
    if(pos.getVelocidade().getY() > 0 && !colisao[COLIDE_Y])
        mudaEstado(CAINDO);
    if(!teclas[DOWN] && estado == ABAIXADO)
    {
        mudaEstado(CORRENDO);
    }

    if (estado == INI_CORRIDA && desenhaFrame.getFrameAtual() == estFrame[estado][1] && desenhaFrame.getTempoEstado() >= tFrame[estado][desenhaFrame.getFrameAtual() - estFrame[estado][0]])
    {
        if (!teclas[LEFT] && !teclas[RIGHT])
        {
            mudaEstado(PARADO);
        }
        else
            mudaEstado(CORRENDO);
    }
    if (estado == VIRANDO_C && desenhaFrame.getFrameAtual() == estFrame[estado][1] && desenhaFrame.getTempoEstado() >= tFrame[estado][desenhaFrame.getFrameAtual() - estFrame[estado][0]])
    {
        if (!teclas[LEFT] && !teclas[RIGHT])
        {
            mudaEstado(PARADO);
        }
        else
            mudaEstado(CORRENDO);
    }
    if (estado == ABAIXANDO && desenhaFrame.getFrameAtual() == estFrame[estado][1] && desenhaFrame.getTempoEstado() >= tFrame[estado][desenhaFrame.getFrameAtual() - estFrame[estado][0]])
    {
        mudaEstado(ABAIXADO);
    }
    if (estado == LEVANTANDO && desenhaFrame.getFrameAtual() == estFrame[estado][1] && desenhaFrame.getTempoEstado() >= tFrame[estado][desenhaFrame.getFrameAtual() - estFrame[estado][0]])
    {
        mudaEstado(PARADO);
    }
    if (estado == C_ESQUIVA && desenhaFrame.getFrameAtual() == estFrame[estado][1] && desenhaFrame.getTempoEstado() >= tFrame[estado][desenhaFrame.getFrameAtual() - estFrame[estado][0]])
    {
        mudaEstado(ESQUIVA);
        audioEnt.tocaAudio("hunterEsquiva", 1, ALLEGRO_PLAYMODE_ONCE, 0);
    }
    /**ATAQUES**/
    if (estado == ATAQUE_1 && desenhaFrame.getFrameAtual() == estFrame[estado][1] && desenhaFrame.getTempoEstado() >= tFrame[estado][desenhaFrame.getFrameAtual() - estFrame[estado][0]])
    {
        mudaEstado(PARADO);
    }
    if (estado == ATAQUE_2 && desenhaFrame.getFrameAtual() == estFrame[estado][1] && desenhaFrame.getTempoEstado() >= tFrame[estado][desenhaFrame.getFrameAtual() - estFrame[estado][0]])
    {
        mudaEstado(PARADO);
    }
    if (estado == ATAQUE_C && desenhaFrame.getFrameAtual() == estFrame[estado][1] && desenhaFrame.getTempoEstado() >= tFrame[estado][desenhaFrame.getFrameAtual() - estFrame[estado][0]])
    {
        mudaEstado(PARADO);
    }

    if (estado == ATAQUE_A && desenhaFrame.getFrameAtual() == estFrame[estado][1] && desenhaFrame.getTempoEstado() >= tFrame[estado][desenhaFrame.getFrameAtual() - estFrame[estado][0]])
    {
        mudaEstado(CAINDO);
    }
    if (estado == ATAQUE_A_2 && desenhaFrame.getFrameAtual() == estFrame[estado][1] && desenhaFrame.getTempoEstado() >= tFrame[estado][desenhaFrame.getFrameAtual() - estFrame[estado][0]])
    {
        mudaEstado(CAINDO);
    }
    if (estado == ATAQUE_A_CI && desenhaFrame.getFrameAtual() == estFrame[estado][1] && desenhaFrame.getTempoEstado() >= tFrame[estado][desenhaFrame.getFrameAtual() - estFrame[estado][0]])
    {
        mudaEstado(CAINDO);
    }



}
void Jogador::criaEfeitos()
{
    if (al_get_timer_count(timerEnt[0]) && estado == ESQUIVA )
    {
        for (int i = 0; i < 6; i++)
        {
            if (!rastJog[i]->getFuncionando())
            {
                rastJog[i]->setPos(pos.getEspaco() + Vetor_R2::vetor(-60, -70));
                rastJog[i]->setVelDes(0.1);
                rastJog[i]->setFrameAtual(desenhaFrame.getFrameAtual());
                rastJog[i]->setAng(dir);
                rastJog[i]->setFuncionando(true);
                break;
            }
        }
        al_set_timer_count(timerEnt[0], 0);
    }
    if (al_get_timer_count(timerEnt[4]) && estado == CORRENDO)
    {
        for (int i = 0; i < 6; i++)
        {
            if (!rastJog[i]->getFuncionando())
            {
                rastJog[i]->setPos(pos.getEspaco() + Vetor_R2::vetor(-60, -70));
                rastJog[i]->setVelDes(0.05);
                rastJog[i]->setFrameAtual(desenhaFrame.getFrameAtual());
                rastJog[i]->setAng(dir);
                rastJog[i]->setFuncionando(true);
                break;
            }
        }
        al_set_timer_count(timerEnt[4], 0);
    }
    if (estado == CORRENDO && (desenhaFrame.getFrameAtual() == 7 || desenhaFrame.getFrameAtual() == 10) && desenhaFrame.getTempoEstado() == 1)
    {
        for(int i = 0; i < 4; i++)
        {
            if (!poeJog[i]->getFuncionando())
            {
                poeJog[i]->setPos(pos.getEspaco());
                poeJog[i]->setDir(dir);
                poeJog[i]->setFuncionando(true);
                break;
            }
        }
        if (mapaAtual->getFase() == 1 && mapaAtual->getPlat(pos.getPlatAtual()).getTipo() == 0)
        {
            audioEnt.tocaAudio("pisadaFloresta", 1, ALLEGRO_PLAYMODE_ONCE, 0);
        }
        else
            audioEnt.tocaAudio("pisadaConcreto", 1, ALLEGRO_PLAYMODE_ONCE, 0);
    }
}
void Jogador::criaProjetil()
{
    if (((estado == ATAQUE_1 && desenhaFrame.getFrameAtual() == 17)||
            (estado == ATAQUE_A && desenhaFrame.getFrameAtual() == 25) ||
            (estado == ATAQUE_2 && desenhaFrame.getFrameAtual() == 36)||
            (estado == ATAQUE_A_2 && desenhaFrame.getFrameAtual() == 43))&&
            desenhaFrame.getTempoEstado() == 1)
    {
        for ( int i = 0; i < 3; i++)
        {
            if (atNoJog[i] != NULL && !atNoJog[i]->getFuncionando())
            {
                if (estado == ATAQUE_A)
                {
                    atNoJog[i]->setPos(pos.getEspaco() + Vetor_R2::vetor(borda.getX()*(1 - 2*dir), -10));
                    atNoJog[i]->setVer(true);
                }
                else if (estado == ATAQUE_1)
                {
                    atNoJog[i]->setPos(pos.getEspaco() + Vetor_R2::vetor(borda.getX()*(1 - 2*dir), -10));
                    atNoJog[i]->setVer(true);
                }
                else if (estado == ATAQUE_2)
                {
                    atNoJog[i]->setPos(pos.getEspaco() + Vetor_R2::vetor(borda.getX()*(1 - 2*dir), -20));
                    atNoJog[i]->setVer(false);
                }
                else if (estado == ATAQUE_A_2)
                {
                    atNoJog[i]->setPos(pos.getEspaco() + Vetor_R2::vetor(borda.getX()*(1 - 2*dir), -20));
                    atNoJog[i]->setVer(false);
                }
                atNoJog[i]->setDi(dir);
                atNoJog[i]->setFuncionando(true);
                break;
            }
        }
        audioEnt.tocaAudio("HunterAtaque1", 0.3, ALLEGRO_PLAYMODE_ONCE, 0);
    }
    if (((estado == ATAQUE_C && desenhaFrame.getFrameAtual() == 52) ||
            (estado == ATAQUE_A_CI && desenhaFrame.getFrameAtual() == 60)) &&
            desenhaFrame.getTempoEstado() == 1)
    {
        for (int i = 0; i < 2; i++)
        {
            if (!atCiJog[i]->getFuncionando())
            {
                atCiJog[i]->setPos(pos.getEspaco() + Vetor_R2::vetor(-12*dir, -40));
                atCiJog[i]->setFuncionando(true);
                break;
            }
        }
        audioEnt.tocaAudio("HunterAtaque1", 0.3, ALLEGRO_PLAYMODE_ONCE, 0);
    }
}
void Jogador::mudaEstado(int nEstado)
{
    int estadoAnt = estado;
    if (estado == PARADO)
    {

    }
    else if(estado == CORRENDO)
    {

    }
    else if(estado == INI_CORRIDA)
    {

    }
    else if(estado == VIRANDO_C)
    {
        if (nEstado == PARANDO)
            nEstado = VIRANDO_C;
    }
    else if(estado == PARANDO)
    {

    }
    else if(estado == DANO)
    {
        tomandoDano = false;
        posDano.setXY(-100,-100);
        desabilitaTeclas[LEFT] = false;
        desabilitaTeclas[RIGHT] = false;
        desabilitaTeclas[DOWN] = false;
        desabilitaTeclas[UP] = false;
        desabilitaTeclas[Z] = false;
        desabilitaTeclas[X] = false;
        desabilitaTeclas[C] = false;
    }
    else if(estadosAtaque(estado))
    {
        habilitaTeclasAtaque(desabilitaTeclas);
    }
    else if(estado == PULANDO)
    {

    }
    else if(estado == CAINDO)
    {

    }
    else if (estado == MORRENDO)
    {

    }
    else if (estado == ABAIXANDO)
    {

    }
    else if (estado == ABAIXADO)
    {
        desabilitaTeclas[LEFT] = false;
        desabilitaTeclas[RIGHT] = false;
        desabilitaTeclas[Z] = false;
    }
    else if (estado == LEVANTANDO)
    {

    }
    else if (estado == C_ESQUIVA)
    {

    }
    else if (estado == ESQUIVA)
    {
        al_stop_timer(timerEnt[2]);
        al_set_timer_count(timerEnt[2], 0);
        al_stop_timer(timerEnt[5]);
        al_set_timer_count(timerEnt[5], 0);
        if (dirEsq)
            pos.getVelocidade().setX(-7);
        else
            pos.getVelocidade().setX(7);
        desabilitaTeclas[LEFT] = false;
        desabilitaTeclas[RIGHT] = false;
        desabilitaTeclas[Z] = false;
        desabilitaTeclas[DOWN] = false;
        desabilitaTeclas[C] = false;
    }
    if (estado != ESQUIVA &&
            ((al_get_timer_started(timerEnt[2]) && !al_get_timer_count(timerEnt[2])) || (al_get_timer_started(timerEnt[5]) && !al_get_timer_count(timerEnt[5]))))
    {
        nEstado = ESQUIVA;
    }
    else if (nEstado != DANO && estadoAnt == DANO && al_get_timer_started(timerEnt[6]) && al_get_timer_count(timerEnt[6]) < 1)
    {
        nEstado = DANO;
        tomandoDano = true;
        permiteColisaoEnt = false;
        desabilitaTeclas[LEFT] = true;
        desabilitaTeclas[RIGHT] = true;
        desabilitaTeclas[DOWN] = true;
        desabilitaTeclas[UP] = true;
        desabilitaTeclas[Z] = true;
        desabilitaTeclas[X] = true;
        desabilitaTeclas[C] = true;
    }

    /**TROCA DE ESTADO**/
    estado = nEstado;
    /**TROCA DE ESTADO**/

    if (estado == PARADO)
    {
        habilitaTeclas(desabilitaTeclas);
    }
    else if(estado == CORRENDO)
    {
        habilitaTeclas(desabilitaTeclas);
    }
    else if(estado == INI_CORRIDA)
    {

    }
    else if(estado == VIRANDO_C)
    {

    }
    else if(estado == PARANDO)
    {

    }
    else if(estado == DANO)
    {
        if (estadoAnt != DANO && !al_get_timer_started(timerEnt[6]))
        {
            if (posDano.getX() > pos.getEspaco().getX())
            {
                pos.getVelocidade().setX(-5);
                dir = 0;
            }
            else
            {
                pos.getVelocidade().setX(5);
                dir = 1;
            }
            pos.getVelocidade().setY(-10);
            pos.getAceleracao(). setX(0);
            colisao[COLIDE_Y] = false;
        }
        al_start_timer(timerEnt[6]);
        permiteColisaoEnt = false;
        desabilitaTeclas[LEFT] = true;
        desabilitaTeclas[RIGHT] = true;
        desabilitaTeclas[DOWN] = true;
        desabilitaTeclas[UP] = true;
        desabilitaTeclas[Z] = true;
        desabilitaTeclas[X] = true;
        desabilitaTeclas[C] = true;
    }
    else if(estadosAtaque(estado))
    {
        desabilitaTeclasAtaque(desabilitaTeclas);
    }
    else if(estado == PULANDO)
    {
        if (estadoAnt == ESQUIVA)
        {
            if (dir == 0)
                pos.getVelocidade().setX(8);
            else
                pos.getVelocidade().setX(-8);
        }
    }
    else if(estado == CAINDO)
    {
        desabilitaTeclas[X] = false;
    }
    else if (estado == MORRENDO)
    {

    }
    else if (estado == ABAIXANDO)
    {

    }
    else if (estado == ABAIXADO)
    {
        desabilitaTeclas[LEFT] = true;
        desabilitaTeclas[RIGHT] = true;
        desabilitaTeclas[Z] = true;
    }
    else if (estado == LEVANTANDO)
    {

    }
    else if (estado == C_ESQUIVA)
    {

    }
    else if (estado == ESQUIVA)
    {
        if (!colisao[COLIDE_Y])
            al_start_timer(timerEnt[5]);
        else
            al_start_timer(timerEnt[2]);

        al_start_timer(timerEnt[3]);

        teclas[C] = false;

        if (dirEsq == 1)
            desabilitaTeclas[LEFT] = true;
        else if (dirEsq == 0)
            desabilitaTeclas[RIGHT] = true;
        desabilitaTeclas[Z] = true;
        desabilitaTeclas[DOWN] = true;
        desabilitaTeclas[C] = true;
    }

}
void Jogador::construiFrames()
{
    int i;
    estFrame = desenhaFrame.getEstFrame();
    tFrame = desenhaFrame.getTFrame();
    estFrame[PARADO][0] = 0 ;
    estFrame[PARADO][1] = 3;
    estFrame[PARADO][2] = 1;
    estFrame[INI_CORRIDA][0] = 4 ;
    estFrame[INI_CORRIDA][1] = 4;
    estFrame[INI_CORRIDA][2] = 0;
    estFrame[CORRENDO][0] = 5 ;
    estFrame[CORRENDO][1] = 10;
    estFrame[CORRENDO][2] = 1;
    estFrame[PARANDO][0] = 11 ;
    estFrame[PARANDO][1] = 11;
    estFrame[PARANDO][2] = 1;
    estFrame[VIRANDO_C][0] = 13 ;
    estFrame[VIRANDO_C][1] = 13;
    estFrame[VIRANDO_C][2] = 0;
    estFrame[PULANDO][0] = 30 ;
    estFrame[PULANDO][1] = 30;
    estFrame[PULANDO][2] = 1;
    estFrame[CAINDO][0] = 31 ;
    estFrame[CAINDO][1] = 31;
    estFrame[CAINDO][2] = 1;
    estFrame[ABAIXANDO][0] = 32 ;
    estFrame[ABAIXANDO][1] = 32;
    estFrame[ABAIXANDO][2] = 0;
    estFrame[ABAIXADO][0] = 33 ;
    estFrame[ABAIXADO][1] = 33;
    estFrame[ABAIXADO][2] = 1;
    estFrame[LEVANTANDO][0] = 32 ;
    estFrame[LEVANTANDO][1] = 32;
    estFrame[LEVANTANDO][2] = 0;

    estFrame[ATAQUE_1][0] = 14 ;
    estFrame[ATAQUE_1][1] = 21;
    estFrame[ATAQUE_1][2] = 0;
    estFrame[ATAQUE_2][0] = 34 ;
    estFrame[ATAQUE_2][1] = 40;
    estFrame[ATAQUE_2][2] = 0;

    estFrame[ATAQUE_A][0] = 23;
    estFrame[ATAQUE_A][1] = 29;
    estFrame[ATAQUE_A][2] = 0;
    estFrame[ATAQUE_A_2][0] = 41;
    estFrame[ATAQUE_A_2][1] = 47;
    estFrame[ATAQUE_A_2][2] = 0;

    estFrame[ATAQUE_A_CI][0] = 58;
    estFrame[ATAQUE_A_CI][1] = 64;
    estFrame[ATAQUE_A_CI][2] = 0;
    estFrame[ATAQUE_C][0] = 49;
    estFrame[ATAQUE_C][1] = 56;
    estFrame[ATAQUE_C][2] = 0;

    estFrame[C_ESQUIVA][0] = 4;
    estFrame[C_ESQUIVA][1] = 4;
    estFrame[C_ESQUIVA][2] = 0;
    estFrame[ESQUIVA][0] = 48;
    estFrame[ESQUIVA][1] = 48;
    estFrame[ESQUIVA][2] = 1;

    estFrame[DANO][0] = 65;
    estFrame[DANO][1] = 65;
    estFrame[DANO][2] = 1;
    estFrame[MORTE][0] = 66;
    estFrame[MORTE][1] = 66;
    estFrame[MORTE][2] = 1;




    tFrame[PARADO].inclui_frente(6);
    tFrame[PARADO].inclui_frente(6);
    tFrame[PARADO].inclui_frente(6);
    tFrame[PARADO].inclui_frente(6);

    tFrame[PULANDO].inclui_frente(10);
    tFrame[CAINDO].inclui_frente(10);

    for (i = estFrame[CORRENDO][0]; i <= estFrame[CORRENDO][1]; i++)
    {
        if ( i == 6 || i == 9)
            tFrame[CORRENDO].inclui_frente(6);
        else
            tFrame[CORRENDO].inclui_frente(5);
    }
    tFrame[VIRANDO_C].inclui_frente(10);



    tFrame[INI_CORRIDA].inclui_frente(3);
    tFrame[PARANDO].inclui_frente(10);

    tFrame[ABAIXANDO].inclui_frente(4);
    tFrame[ABAIXADO].inclui_frente(5);
    tFrame[LEVANTANDO].inclui_frente(4);

    tFrame[ESQUIVA].inclui_frente(1);
    tFrame[C_ESQUIVA].inclui_frente(5);

    tFrame[DANO].inclui_frente(1);

    /**ATAQUES**/
    tFrame[ATAQUE_1].inclui_frente(3);
    tFrame[ATAQUE_1].inclui_frente(5);
    tFrame[ATAQUE_1].inclui_frente(3);
    tFrame[ATAQUE_1].inclui_frente(2);
    tFrame[ATAQUE_1].inclui_frente(2);
    tFrame[ATAQUE_1].inclui_frente(6);
    tFrame[ATAQUE_1].inclui_frente(4);
    tFrame[ATAQUE_1].inclui_frente(2);

    tFrame[ATAQUE_A].inclui_frente(2);
    tFrame[ATAQUE_A].inclui_frente(3);
    tFrame[ATAQUE_A].inclui_frente(2);
    tFrame[ATAQUE_A].inclui_frente(3);
    tFrame[ATAQUE_A].inclui_frente(2);
    tFrame[ATAQUE_A].inclui_frente(6);
    tFrame[ATAQUE_A].inclui_frente(4);
    tFrame[ATAQUE_A].inclui_frente(2);

    tFrame[ATAQUE_2].inclui_frente(3);
    tFrame[ATAQUE_2].inclui_frente(3);
    tFrame[ATAQUE_2].inclui_frente(2);
    tFrame[ATAQUE_2].inclui_frente(2);
    tFrame[ATAQUE_2].inclui_frente(6);
    tFrame[ATAQUE_2].inclui_frente(4);
    tFrame[ATAQUE_2].inclui_frente(2);

    tFrame[ATAQUE_A_2].inclui_frente(2);
    tFrame[ATAQUE_A_2].inclui_frente(3);
    tFrame[ATAQUE_A_2].inclui_frente(3);
    tFrame[ATAQUE_A_2].inclui_frente(2);
    tFrame[ATAQUE_A_2].inclui_frente(6);
    tFrame[ATAQUE_A_2].inclui_frente(4);
    tFrame[ATAQUE_A_2].inclui_frente(2);

    tFrame[ATAQUE_A_CI].inclui_frente(2);
    tFrame[ATAQUE_A_CI].inclui_frente(3);
    tFrame[ATAQUE_A_CI].inclui_frente(2);
    tFrame[ATAQUE_A_CI].inclui_frente(3);
    tFrame[ATAQUE_A_CI].inclui_frente(2);
    tFrame[ATAQUE_A_CI].inclui_frente(6);
    tFrame[ATAQUE_A_CI].inclui_frente(4);
    tFrame[ATAQUE_A_CI].inclui_frente(2);

    tFrame[ATAQUE_C].inclui_frente(4);
    tFrame[ATAQUE_C].inclui_frente(5);
    tFrame[ATAQUE_C].inclui_frente(3);
    tFrame[ATAQUE_C].inclui_frente(2);
    tFrame[ATAQUE_C].inclui_frente(2);
    tFrame[ATAQUE_C].inclui_frente(6);
    tFrame[ATAQUE_C].inclui_frente(2);
    tFrame[ATAQUE_C].inclui_frente(2);

    tFrame[MORTE].inclui_frente(2);

}
void Jogador::setMapaAtual(Mapa* m)
{
    mapaAtual = m;
}
void Jogador::setControle(Controle* c)
{
    contr = c;
}
void Jogador::inicializaEntidade()
{
    for (int i = 0; i < 3; i++)
    {
        atNoJog[i] = new Ataque_Normal_Jogador(Vetor_R2::vetor(-200,-200), 0, false);
        atNoJog[i]->setMapaAtual(mapaAtual);
        atNoJog[i]->setPtrJ(NULL);
        atNoJog[i]->setListaEnt(listEnt);
        listEnt->incluiEntidade(static_cast<Entidade*> (atNoJog[i]));
    }
    for (int i = 0; i < 2 ; i++)
    {
        atCiJog[i] = new Ataque_Cima_Jogador(Vetor_R2::vetor(-200,-200));
        atCiJog[i]->setMapaAtual(mapaAtual);
        atCiJog[i]->setPtrJ(NULL);
        atCiJog[i]->setListaEnt(listEnt);
        listEnt->incluiEntidade(static_cast<Entidade*> (atCiJog[i]));
    }
    for (int i = 0; i <4; i ++)
    {
        poeJog[i] = new Poeira(Vetor_R2::vetor(-200,-200), borda, 1, false, 0, true, 0.05);
        poeJog[i]->setMapaAtual(mapaAtual);
        poeJog[i]->setPtrJ(NULL);
        poeJog[i]->setListaEnt(listEnt);
        listEnt->incluiEntidade(static_cast<Entidade*> (poeJog[i]));
    }
    for (int i = 0; i < 6; i++)
    {
        rastJog[i] = new Rastro(Vetor_R2::vetor(-200, -200), desenhaFrame.getImagem(),
                                desenhaFrame.getSpriteX(), desenhaFrame.getSpriteY(),
                                2, 0.1, 1, dir, (1 - numJogador), 1, 1);

        rastJog[i]->setMapaAtual(mapaAtual);
        rastJog[i]->setPtrJ(NULL);
        rastJog[i]->setListaEnt(listEnt);
        listEnt->incluiEntidade(static_cast<Entidade*> (rastJog[i]));
    }
}
int Jogador::getNumJogador()
{
    return numJogador;
}
int Jogador::getEstadoJogador()
{
    return estado;
}
int Jogador::getMorreu()
{
    return morreu;
}
void Jogador::resetaEntidade()
{
    morreu = false;
    vida = vidaBase + rand()%(vidaRand*2 + 1) - vidaRand;
    alphaMorte = 255;
    estado = PARADO;
    desenhaFrame.setEstado(PARADO);
    desenhaFrame.atualizaFrame();
    pos.setPlatAtual(0);
    al_stop_timer(timerEnt[0]);
    al_stop_timer(timerEnt[1]);
    al_stop_timer(timerEnt[2]);
    al_stop_timer(timerEnt[3]);
    al_stop_timer(timerEnt[4]);
    al_stop_timer(timerEnt[5]);
    al_stop_timer(timerEnt[6]);
    al_set_timer_count(timerEnt[0], 0);
    al_set_timer_count(timerEnt[1], 0);
    al_set_timer_count(timerEnt[2], 0);
    al_set_timer_count(timerEnt[3], 0);
    al_set_timer_count(timerEnt[4], 0);
    al_set_timer_count(timerEnt[5], 0);
    al_set_timer_count(timerEnt[6], 0);

    corEnt = al_map_rgba(255*(1 - numJogador),255,255,255);
    permiteColisaoEnt = true;
    permiteColisaoChao = true;

    teclas[C] = false;
    teclas[DOWN] = false;
    teclas[X] = false;
    teclas[Z] = false;
    teclas[RIGHT] = false;
    teclas[LEFT] = false;
    teclas[UP] = false;

    al_start_timer(timerEnt[0]);
    al_start_timer(timerEnt[4]);


}
void Jogador::paraTimer()
{
    for (int i = 0; i < 7; i++)
    {
        if (al_get_timer_started(timerEnt[i]))
        {
            al_stop_timer(timerEnt[i]);
            timerParado.inclui_frente(i);
        }
    }
}
void Jogador::setControleExterno(bool c)
{
    controleExterno = c;
}
