#ifndef CHEFAO_H
#define CHEFAO_H
#include "Golem.h"
class Chuva_Fogo;

class Onda_Fogo;

class Coluna_Fogo;
class Rastro;

class Chefao : public Inimigo
{
public:
    Chefao();
    Chefao(int ix,int iy);

    ~Chefao();

    void desenha();
    void atualiza();
    void criaProjetil();
    void construiFrames();
    void atualizaComportamento();
    void atualizaColisoes();
    void atualizaEstados();
    void criaEfeitos();
    void setFuncionando(bool f);
    void inicializaEntidade();
    void resetaEntidade();

    void paraTimer();

    bool desviar();

private:
    Jogador* alvo;
    float velXDano, velYDano;
    float delX,delY,denominador;
    int fade;
    bool perto;
    float vXMax;
    float destino;
    float destinoY;
    bool teleport;
    bool teleportConj;
    int dirDestino;
    bool iniciaDash;
    int tempoDash;
    int especiaisChefao;
    bool iniciarEspecial;
    Ataque_Generico* atkChefe[7];
    Chuva_Fogo* chuvaProj[15];
    Onda_Fogo* ondaProj[7];
    Coluna_Fogo* colunaProj[10];
    Rastro* rast[10];
    int danoDash;
    int danoRandDash;
};

#endif // CHEFAO_H
