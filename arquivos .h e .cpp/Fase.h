#ifndef FASE_H
#define FASE_H
#include "Lista_Entidades.h"

const int NUM_FUMACA = 20;
const int NUM_EFEITO_DANO = 50;
class Fumaca;
class Efeito_Dano;
class Jogo;

class Fase
{
    public:
        Fase();
        virtual ~Fase();

        virtual void atualiza() = 0;
        virtual void desenha() = 0;
        virtual void inicializaFase(Controle * control) = 0;
        virtual void inicializaInimigo() = 0;
        virtual void inicializaObstaculos() = 0;
        virtual void inicializaPlataformas() = 0;
        void inicializaFumaca();
        void passaFase(int i);

        void setJogo(Jogo* j);
        Jogador* getJog(int i);

        void atualizaEDano();
        void desenhaEDano();
        void inicializaEDano();
        void criaDano(Vetor_R2 esp, Vetor_R2 b, int dano);

        void paraMusica();
        void paraTimer();
        void ativaTimer();

    protected:
        string nomeImgFase;

        int numPlat;
        int **platX, **platY;
        int *tipoPlat;

        int numObs;
        int *obsX, *obsY;
        int *tipoObs;

        int numInimigosComeco;
        int numIniInvocados;
        int *iniX, *iniY;
        int *tipoIni;

        Mapa* ptrM;
        Jogador* ptrJ[2];
        Jogo * jogo;
        Controle *contr;
        Lista_Entidades *listE;
        Lista_Generica<Fumaca*> listFum;
        Lista_Generica<Efeito_Dano*> listEDano;
        Lista_Generica<Entidade*>* listGEnt;
        ALLEGRO_TIMER ** timers;
        Audio audioFase;
        bool comecouFase;
        int alpha;
        int fade;
        int posX;
        bool doisJog;
        bool audioTocando;
        bool timerParado;
        Lista_Generica<int> timersParados;


};

#endif // FASE_H
