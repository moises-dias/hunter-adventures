#ifndef BOTAO_H
#define BOTAO_H
#include "Sprite.h"
#include "CEI.h"
enum NOME_BOTOES{NOVO_JOGO, CARREGAR, OPCOES, SAIR, UM_JOG, DOIS_JOG, FACIL, MEDIO, DIFICIL};

class Botao
{
public:
    Botao();
    ~Botao();
    void inicializa(int i);
    void atualiza();
    void desenha(int i, ALLEGRO_COLOR cor);

private:
    Sprite botaoJogar;
    Vetor_R2 posicaoBotao[9];
    Vetor_R2 referencia;
};

#endif // BOTAO_H
