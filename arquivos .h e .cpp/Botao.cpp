#include "Botao.h"

Botao::Botao()
{
    referencia.setXY(0,0);
    posicaoBotao[NOVO_JOGO].setXY(780,20);
    posicaoBotao[CARREGAR].setXY(780,130);
    posicaoBotao[OPCOES].setXY(780,240);
    posicaoBotao[SAIR].setXY(780,350);
    posicaoBotao[UM_JOG].setXY(780,20);
    posicaoBotao[DOIS_JOG].setXY(780,130);
    posicaoBotao[FACIL].setXY(780,20);
    posicaoBotao[MEDIO].setXY(780,130);
    posicaoBotao[DIFICIL].setXY(780,240);
}
Botao::~Botao()
{

}
void Botao::inicializa(int i)
{
    botaoJogar.setNumEstados(1);
    botaoJogar.setNomeImg("imagemjogar.png");
    botaoJogar.setSpriteX(300);
    botaoJogar.setSpriteY(100);

    botaoJogar.setEstado(0);

    Lista_Generica<int>* tFrame = botaoJogar.getTFrame();
    for(int j=0;j<6;j++)
        tFrame[0].inclui_frente(20);

    int** estFrame = botaoJogar.getEstFrame();
        estFrame[0][0]=i*10;
        estFrame[0][1]=5+i*10;
        estFrame[0][2]=1;
    atualiza();
}
void Botao::atualiza()
{
    botaoJogar.atualizaFrame();
}
void Botao::desenha(int i, ALLEGRO_COLOR cor)
{
    botaoJogar.desenha(posicaoBotao[i],referencia,0,0,cor);
}
