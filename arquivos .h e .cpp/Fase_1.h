#ifndef FASE_1_H
#define FASE_1_H
#include "Fase.h"
#include "Inimigos_Fase_1.h"



const int NUM_SLIME_FASE_1 = 3;
const int NUM_BESOURO_FASE_1 = 3;
const int NUM_GOBLIN_FASE_1 = 3;
const int NUM_MORCEGO_FASE_1 = 3;
const int NUM_ESQUELETO_FASE_1 = 3;

const int NUM_ARMADILHAS = 18;

class Fumaca;
class Barra_De_Vida;
class Jogo;
class Armadilha_Urso;

class Fase_1 : public Fase
{
    public:
        Fase_1();
        Fase_1(Lista_Entidades * listEnt, bool doisJ, Controle * control);

        ~Fase_1();

        void atualiza();
        void desenha();
        void inicializaFase(Controle * control);
        void inicializaInimigo();
        void inicializaObstaculos();
        void inicializaPlataformas();
        void paraFase(float x1, float x2);
        void carregaInimigos(int t);
        void resetaArmadilhas();

        void pontoParada1();
        void pontoParada2();
        void invocaInimigo(int t, Vetor_R2 p);

        void atualizaCheckpoint(int c);

    private:
        int pontoParada;
        bool podePassar;
        int progressao;
        bool invocou;
        bool terminouInvocar;
        ALLEGRO_BITMAP *glow;
        ALLEGRO_BITMAP *dGlow;
        Lista_Generica<Slime*> listSlime;
        Lista_Generica<Besouro*> listBesouro;
        Lista_Generica<Goblin*> listGoblin;
        Lista_Generica<Morcego*> listMorcego;
        Lista_Generica<Esqueleto*> listEsqueleto;

        Lista_Generica<Armadilha_Urso*> listArmadilhas;

        Lista_Generica<Inimigo*> listIniPonto;
        Lista_Generica<int>::Iterador iteP1;
        Lista_Generica<int>::Iterador iteP2;

        Inimigos_Fase_1 inimigos;
        int ang;

        Barra_De_Vida** ptrBV;

};

#endif // FASE_1_H
