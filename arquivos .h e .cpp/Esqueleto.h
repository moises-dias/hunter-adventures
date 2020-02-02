#include "Inimigo.h"

class Osso_Aereo;
class Ataque_Generico;
#ifndef ESQUELETO_H
#define ESQUELETO_H

class Lista_Entidades;

class Esqueleto : public Inimigo
{
public:
    Esqueleto();
    Esqueleto(int ix,int iy);
    ~Esqueleto();

    void desenha();
    void atualiza();
    void criaProjetil();
    void construiFrames();
    void atualizaComportamento();
    void atualizaColisoes();
    void atualizaEstados();
    void criaEfeitos();
    void setDir(float d);
    void setTipo(bool t);

    void inicializaEntidade();
    void resetaEntidade();
    void setFuncionando(bool f);

    void paraTimer();

private:
    Jogador* alvo;
    float aceleracao;
    bool atirar;
    bool perto;
    float yPlat;
    float delXPulo;
    float vXMax;
    float vRand;

    bool tipo;/**1 = invocado, 0 = comum**/
    Osso_Aereo* ossos[3];
    Ataque_Generico* atkOsso[2];
};

#endif // ESQUELETO_H
