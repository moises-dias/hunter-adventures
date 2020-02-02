
#include "Golem_Maligno.h"
#ifndef MORCEGO_H
#define MORCEGO_H

class Lista_Entidades;

class Morcego : public Inimigo
{
public:
    Morcego();
    Morcego(int ix,int iy);
    ~Morcego();

    void desenha();
    void atualiza();
    void criaProjetil();
    void construiFrames();
    void atualizaComportamento();
    void atualizaColisoes();
    void atualizaEstados();
    void criaEfeitos();

    void inicializaEntidade();
    void resetaEntidade();
    void setFuncionando(bool f);

    void paraTimer();

private:
    Jogador* alvo;
    float ang;
    float vmax;
    float aceleracao;
    bool atirar;
    int tempoCarrega;
    int delayRasante;
    Vetor_R2 velRasante;
    float delX,delY,velX,velY,tempo;
    float yInic;
    float velXDano;
    float velYDano;
    float distY;

};

#endif // MORCEGO_H
