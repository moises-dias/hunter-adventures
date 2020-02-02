#ifndef FASE_2_H
#define FASE_2_H
#include "Fase_1.h"
#include "Inimigos_Fase_2.h"

const int NUM_SLIME_FASE_2 = 3;
const int NUM_GOBLIN_FASE_2 = 3;
const int NUM_MORCEGO_FASE_2 = 3;
const int NUM_SLIME_LAVA_FASE_2 = 3;
const int NUM_ESQUELETO_FASE_2 = 3;
const int NUM_FOGO_FATUO_FASE_2 = 3;
const int NUM_GOLEM_FASE_2 = 3;

const int NUM_LAVA = 3;

class Fumaca;
class Barra_De_Vida;
class Jogo;
class Lava;

class Fase_2 : public Fase
{
    public:
        Fase_2();
        ~Fase_2();
        Fase_2(Lista_Entidades * listEnt, bool doisJ, Controle * control);

        void atualiza();
        void desenha();
        void inicializaFase(Controle * control);
        void inicializaInimigo();
        void inicializaObstaculos();
        void inicializaPlataformas();
        void paraFase(float x1, float x2);
        void carregaInimigos(int t);
        void resetaObstaculos();

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
        Lista_Generica<Goblin*> listGoblin;
        Lista_Generica<Morcego*> listMorcego;
        Lista_Generica<Lava_Slime*> listLavaSlime;
        Lista_Generica<Esqueleto*> listEsqueleto;
        Lista_Generica<Golem*> listGolem;
        Lista_Generica<Fogo_Fatuo*> listFogoFatuo;

        Lista_Generica<Lava*> listLava;

        Lista_Generica<Inimigo*> listIniPonto;
        Lista_Generica<int>::Iterador iteP1;
        Lista_Generica<int>::Iterador iteP2;

        Inimigos_Fase_2 inimigos;
        int ang;

        Barra_De_Vida** ptrBV;
};

#endif // FASE_2_H
