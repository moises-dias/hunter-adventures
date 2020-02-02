#include "Besouro.h"
#ifndef NECROMANTE_H
#define NECROMANTE_H

class Caveira_Flamejante;

class Necromante : public Inimigo
{
public:
    Necromante();
    Necromante(int ix,int iy);
    ~Necromante();

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
    Esqueleto* esqueletos[2];
    Caveira_Flamejante* caveiras[5];
    int fade;
    float delX,delY,denominador;
    float velXDano, velYDano;
    float distX,distY;
    float vXMax;
    float vYMax;
    int tempoProjetil;
};

#endif // NECROMANTE_H
