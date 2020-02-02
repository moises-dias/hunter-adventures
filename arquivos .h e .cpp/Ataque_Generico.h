#ifndef ATAQUE_GENERICO_H
#define ATAQUE_GENERICO_H
#include "Projetil.h"

class Ataque_Generico : public Projetil
{
public:
    Ataque_Generico();
    Ataque_Generico(Vetor_R2 posI, Vetor_R2 borda_ataque, int dano_ataque, int tempo_ativo);
    ~Ataque_Generico();

    void desenha();
    void atualiza();
    void criaFrames();

    void setBorda(Vetor_R2 b);
    void setTempoMax(int t);
    void setDano(int d);

    void resetaEntidade();

private:
    int tempoMax;
    int tempoAtual;
};

#endif // ATAQUE_GENERICO_H
