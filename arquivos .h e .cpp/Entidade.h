#ifndef ENTIDADE_H
#define ENTIDADE_H
#include "Sprite.h"
#include "Posicao.h"
#include "Audio.h"
#include "CEI.h"

class Lista_Entidades;

class Entidade
{
protected:
    int ID;
    bool permiteColisaoEnt;
    bool permiteColisaoChao;
    bool tomandoDano;
    int estado;/**caindo, tomando dano.. "gravitacional", "interacao"**/
    bool* colisao;
    Posicao pos;
    Vetor_R2 borda;
    Vetor_R2 posDano;
    Vetor_R2 posVelo;
    Sprite desenhaFrame;
    ALLEGRO_TIMER** timerEnt;
    ALLEGRO_COLOR corEnt;
    Lista_Entidades * listEnt;
    int ** estFrame;
    Lista_Generica<int> * tFrame;
    bool colidePlat;
    Audio audioEnt;

    int vidaBase, danoBase;
    int vidaRand, danoRand;
    int vida, dano; /**danos diferentes para ataques diferentes**/
    float dir;
    int proj;

    bool funcionando;
    Lista_Generica<int> timerParado;


public:
    Entidade();
    Entidade(int sX, int sY, char* nomeImg, int numEstados);
    virtual ~Entidade();

    virtual void desenha()=0;
    virtual void atualiza()=0;

    virtual void setFuncionando(bool f);
    virtual bool getFuncionando();
    virtual void paraTimer(){}
    virtual void ativaTimer();
    void mudaScore(int s);

    int getID();
    void setProj(int i);
    int getProj();
    void setEstado(int e);
    int getEstado();
    void setPermiteColisaoEnt(bool c);
    bool getPermiteColisaoEnt();
    void setPermiteColisaoChao(bool c);
    bool getPermiteColisaoChao();
    void setTomandoDano(bool t);
    bool getTomandoDano();
    void setColisao(bool c, int i);
    void setListaEnt(Lista_Entidades * l);
    Posicao& getPos();
    Vetor_R2& getBorda();
    Vetor_R2 getPosDano();
    Vetor_R2 getPosVelo();
    void setPosDano(Vetor_R2 d);
    void setPosVelo(Vetor_R2 v);

    void setCorEnt(ALLEGRO_COLOR c);
    ALLEGRO_COLOR getCorEnt();

    Sprite& getSprite();

    bool getColidePlat();
    float getDir();
    virtual void resetaEntidade();

    int getVida();
    int getDano();
    void diminuiVida(int d);

    static ALLEGRO_FONT * font16;



};

#endif // ENTIDADE_H
