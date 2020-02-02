#include "Menu_Pausa.h"

Menu_Pausa::Menu_Pausa()
{

}

Menu_Pausa::~Menu_Pausa()
{
    al_destroy_bitmap(botao);
    al_destroy_bitmap(cursor);
    botao = NULL;
    cursor = NULL;
}
Menu_Pausa::Menu_Pausa(Controle * pC)
{
    ptrControl = pC;



    alpha = 0;
    opcao = 0;
    fade = 4;
    escala = 0.3;
    mudaEstado = false;
    estado = PLAY;

    tamanhoSB.setXY(290, 100);
    tamanhoSC.setXY(34,32);

    for (int i = 0; i < 3; i++)
    {
        posBotao[i].setXY(1200 - tamanhoSB.getX()*escala - 20, 20 + i*(tamanhoSB.getY()*escala + 20));
    }
    posCursor.setXY(posBotao[0].getX() - tamanhoSC.getX() - 20,
                    posBotao[0].getY() + tamanhoSB.getY()*escala/2 - tamanhoSC.getY()/2);

}

void Menu_Pausa::atualiza()
{
    if (fade == 0)
    {
        if (ptrControl->getTeclas(UP))
        {
            opcao--;
            audioPausa.tocaAudio("CURSOR_MUDAR", 0.7, ALLEGRO_PLAYMODE_ONCE, 0);
            ptrControl->setTeclas(UP, false);
        }
        if (ptrControl->getTeclas(DOWN))
        {
            opcao++;
            audioPausa.tocaAudio("CURSOR_MUDAR", 0.7, ALLEGRO_PLAYMODE_ONCE, 0);
            ptrControl->setTeclas(DOWN, false);
        }
        if (opcao > 2)
            opcao = 0;
        if (opcao < 0)
            opcao = 2;

        posCursor.setXY(posBotao[opcao].getX() - tamanhoSC.getX() - 20,
                    posBotao[opcao].getY() + tamanhoSB.getY()*escala/2 - tamanhoSC.getY()/2);
        if (opcao == 0 && ptrControl->getTeclas(X))
        {
            ptrControl->setTeclas(X, false);
            estado = PLAY;
            fade = -4;
        }
        else if (opcao == 1 && ptrControl->getTeclas(X))
        {
            ptrControl->setTeclas(X, false);
            estado = MENU;
            fade = -4;
        }
        else if (opcao == 2 && ptrControl->getTeclas(X))
        {
            ptrControl->setTeclas(X, false);
            estado = FECHA;
            fade = -4;
        }
    }
    else
    {
        alpha += fade;
        if (alpha >= 255 && fade == 4)
        {
            alpha = 255;
            fade = 0;
        }
        if (alpha <= 0 && fade == -4)
        {
            alpha = 0;
            fade = 0;
            opcao = 0;
            mudaEstado = true;
        }

    }
    audioPausa.atualizaAudio();

}
void Menu_Pausa::desenha()
{
    al_draw_tinted_scaled_bitmap(glow, al_map_rgba(alpha,alpha,alpha,alpha), 0,0,948,666, 0,0,1200,720, 0);
    for (int i = 0, j = 0; i < 3; i++)
    {
        if (i == 1)
            j = tamanhoSB.getX();
        al_draw_tinted_scaled_bitmap(botao, al_map_rgba(alpha, alpha, alpha, alpha),
                                     tamanhoSB.getX()*i + j, 0, tamanhoSB.getX(), tamanhoSB.getY(),
                                     posBotao[i].getX(), posBotao[i].getY(), tamanhoSB.getX()*escala, tamanhoSB.getY()*escala, 0);
    }

    al_draw_tinted_scaled_bitmap(cursor,al_map_rgba(alpha, alpha, alpha, alpha),
                                 0,0,tamanhoSC.getX(), tamanhoSC.getY(),
                                 posCursor.getX(), posCursor.getY(), tamanhoSC.getX(), tamanhoSC.getY(),0);

}
void Menu_Pausa::inicializa()
{
    botao = al_load_bitmap("Menu_Pausa.png");
    cursor = al_load_bitmap("Menu_Cursor.png");
    glow = al_load_bitmap("darkGlow2.png");
    string nome[] = {"CURSOR_MUDAR","CURSOR_NAO_PERMITIR", "CURSOR_SELECIONAR"};
    audioPausa.inicializaAudio(nome, 3);
}
void Menu_Pausa::setControle(Controle* pC)
{
    ptrControl = pC;
}
void Menu_Pausa::resetaPausa()
{
    alpha = 0;
    opcao = 0;
    fade = 4;
    estado = PLAY;
    mudaEstado = false;
    posCursor.setXY(posBotao[opcao].getX() - tamanhoSC.getX() - 20,
                    posBotao[opcao].getY() + tamanhoSB.getY()*escala/2 - tamanhoSC.getY()/2);
    audioPausa.atualizaAudio();
}
bool Menu_Pausa::getMudarEstado()
{
    return  mudaEstado;
}
int Menu_Pausa::getEstado()
{
    return estado;
}
