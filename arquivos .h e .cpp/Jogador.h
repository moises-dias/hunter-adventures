#include "Mapa.h"
#include "Controle.h"

#ifndef JOGADOR_H
#define JOGADOR_H

class Lista_Entidades;
class Poeira;
class Rastro;
class Ataque_Normal_Jogador;
class Ataque_Cima_Jogador;


class Jogador : public Entidade
{
private:

    Mapa* mapaAtual;
    Controle * contr;
    bool * teclas;
    int numJogador;
    bool combo;
    bool podeAT; // Bool para permitir ataque aéreo
    bool podeAA; //Bool para permitir ataque aéreo
    int dirEsq;
    int estado;
    bool desabilitaTeclas[NUM_KEYS];
    int numAtaquesAereos;
    bool morreu;
    int alphaMorte;

    bool controleExterno;
    Ataque_Cima_Jogador * atCiJog[2];
    Ataque_Normal_Jogador * atNoJog[3];
    Rastro * rastJog[6];
    Poeira * poeJog[4];

public:
    Jogador();
    Jogador(int nJ);
    ~Jogador();
    void construiFrames();
    void mudaEstado(int nEstado);
    void setMapaAtual(Mapa* m);
    void setControle(Controle* c);
    void desenha();
    void atualiza();
    void movimentacao();
    void esquiva();
    void ataques();
    void atualizaComportamento();
    void atualizaColisoes();
    void atualizaEstados();
    void criaEfeitos();
    void criaProjetil();
    void inicializaEntidade();
    void setControleExterno(bool c);

    int getNumJogador();
    int getEstadoJogador();
    int getMorreu();

    void resetaEntidade();
    void paraTimer();


};

#endif // JOGADOR_H
