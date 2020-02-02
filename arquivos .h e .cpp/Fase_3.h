#ifndef FASE_3_H
#define FASE_3_H
#include "Fase_2.h"
#include "Inimigos_Fase_3.h"

const int NUM_SLIME_LAVA_3 = 3;
const int NUM_FOGO_FATUO_3 = 3;
const int NUM_ESQUELETO_3 = 3;
const int NUM_GOLEM_3 = 3;
const int NUM_GOLEM_MALIGNO_3 = 3;
const int NUM_NECROMANTE_3 = 3;

const int NUM_FANTASMA = 3;


class Fumaca;
class Barra_De_Vida;
class Jogo;
class Fantasma;

class Fase_3 : public Fase
{
    public:
        Fase_3();
        ~Fase_3();
        Fase_3(Lista_Entidades * listEnt, bool doisJ, Controle * control);

        void atualiza();
        void desenha();
        void inicializaFase(Controle * control);
        void inicializaInimigo();
        void inicializaObstaculos();
        void inicializaPlataformas();
        void paraFase(float x1, float x2);
        void carregaInimigos(int t);
        void lutaChefao();

        void resetaObstaculos();
        void pontoParada1();
        void invocaInimigo(int t, Vetor_R2 p);

        void atualizaCheckpoint(int c);

    private:
        int pontoParada;
        bool podePassar;
        int progressao;
        bool invocou;
        bool chefaoMorto;
        bool chefaoInvocado;
        bool areaChefao;
        bool terminouInvocar;
        ALLEGRO_BITMAP *glow;
        ALLEGRO_BITMAP *dGlow;
        Lista_Generica<Lava_Slime*> listLavaSlime;
        Lista_Generica<Fogo_Fatuo*> listFogoFatuo;
        Lista_Generica<Esqueleto*> listEsqueleto;
        Lista_Generica<Golem*> listGolem;
        Lista_Generica<Golem_Maligno*> listGolemMaligno;
        Lista_Generica<Necromante*> listNecromante;

        Lista_Generica<Fantasma*> listFantasma;

        Chefao* ptrChefao;
        Inimigos_Fase_3 inimigos;
        int ang;

        Lista_Generica<Inimigo*> listIniPonto;
        Lista_Generica<int>::Iterador iteP1;

        Barra_De_Vida** ptrBV;

};

#endif // FASE_3_H
