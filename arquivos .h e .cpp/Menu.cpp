#include "Menu.h"

Menu::Menu()
{
    for(int i=0; i<4; i++)
        permiteOpcao[i]=true;
    menuAtual=0;
    opcaoSelecionada=0;
    cor=0;
    iniciando=true;
    finalizando=false;
    escalaCursor = 1.8;
    posXMenu = 800;
    posYMenu = 10;


}
Menu::~Menu()
{

}
void Menu::inicializa()
{
    fontMenu = al_load_font("moolbor_0.ttf", 80, 0);
    fontRank = al_load_font("moolbor_0.ttf", 40, 0);
    telaMenu=al_load_bitmap("Menu_Tela.png");
    cursor=al_load_bitmap("Menu_Cursor.png");
    string nomeA[] = {"MUSICA_MENU", "CURSOR_MUDAR", "CURSOR_SELECIONAR", "CURSOR_VOLTAR",
                     "CURSOR_NAO_PERMITIR","CURSOR_INICIAR_JOGO"
                    };
    audioMenu.inicializaAudio(nomeA, 6);

    if (nome.size())
        nome.clear();
    opcoesMenu[0].inclui_frente("Jogar");
    opcoesMenu[0].inclui_frente("Opcoes");
    opcoesMenu[0].inclui_frente("Ranking");
    opcoesMenu[0].inclui_frente("Sair");
    opcoesMenu[1].inclui_frente("Save1");
    opcoesMenu[1].inclui_frente("Save2");
    opcoesMenu[1].inclui_frente("Save3");
    opcoesMenu[2].inclui_frente("Um Jogador");
    opcoesMenu[2].inclui_frente("Dois Jogadores");
    opcoesMenu[3].inclui_frente("Continuar");
    opcoesMenu[3].inclui_frente("Selecionar Fase");
    opcoesMenu[4].inclui_frente("Fase 1");
    opcoesMenu[4].inclui_frente("Fase 2");
    opcoesMenu[4].inclui_frente("Fase 3");
    opcoesMenu[5].inclui_frente("Deletar Save");
    opcoesMenu[6].inclui_frente("Save 1");
    opcoesMenu[6].inclui_frente("Save 2");
    opcoesMenu[6].inclui_frente("Save 3");
    opcoesMenu[7].inclui_frente("Ranking");
    opcoesMenu[8].inclui_frente("Escolha Seu Nome");

}
void Menu::desenhaMenu()
{
    if(!audioMenu.audioTocando("MUSICA_MENU") && !finalizando)
        audioMenu.tocaAudio("MUSICA_MENU",1,ALLEGRO_PLAYMODE_LOOP,0);
    if(iniciando&&cor<=255)
        cor+=1;
    if(finalizando&&cor>=0)
        cor-=1;
    if(cor>=255 || cor<=0)
    {
        if(cor>255)
            cor=255;
        if(cor<0)
            cor=0;
        iniciando=false;
    }

    al_draw_tinted_scaled_bitmap(telaMenu,al_map_rgba(cor,cor,cor,cor),0,0,910,703,0,0,1200,720,0);

    iteMenu = opcoesMenu[menuAtual].inicio();
    for (int i = 0; i < opcoesMenu[menuAtual].tamanho(); i++, iteMenu++)
    {
        if (menuAtual != 8)
            al_draw_text(fontMenu, al_map_rgba(255 - cor,255 -cor, 255 - cor,cor), posXMenu, posYMenu + i*al_get_font_ascent(fontMenu),
                        0, (*iteMenu).c_str());
        if (menuAtual == 7)
        {
            Lista_Generica<string>::Iterador iteNome;
            iteNome = nomeRanking->inicio();
            Lista_Generica<int>::Iterador iteScore;
            iteScore = scoreRanking->inicio();
            for (int j = 0; j < scoreRanking->tamanho(); j++,iteNome++, iteScore++)
            {
                al_draw_textf(fontRank, al_map_rgb(0,0,0), posXMenu + al_get_text_width(fontMenu, "Ranking")/2,
                              posYMenu + 10 + al_get_font_ascent(fontMenu) + j*al_get_font_ascent(fontRank), ALLEGRO_ALIGN_CENTER,
                              "%i - %s - %i", j+1, (*iteNome).c_str(), (*iteScore));
            }
        }
        else if (menuAtual == 8)
        {
            al_draw_text(fontMenu, al_map_rgba(255 - cor,255 -cor, 255 - cor,cor), posXMenu - 100, posYMenu + i*al_get_font_ascent(fontMenu),
                        0, (*iteMenu).c_str());
            al_draw_textf(fontRank, al_map_rgb(0,0,0), posXMenu + - 100 +  al_get_text_width(fontMenu, "Escolha Seu Nome")/2,
                          posYMenu + 10 + al_get_font_ascent(fontMenu), ALLEGRO_ALIGN_CENTER, "%s", nome.c_str());
        }
    }
    if(cor==255)
        al_draw_tinted_scaled_bitmap(cursor, al_map_rgba(cor,cor,cor,cor), !permiteOpcao[opcaoSelecionada]*34, 0, 34,32,
                                     posXMenu - 34*escalaCursor - 10, posYMenu + (al_get_font_line_height(fontMenu) +
                                     2*opcaoSelecionada*al_get_font_ascent(fontMenu) - 34*escalaCursor)/2,
                                     escalaCursor*34, escalaCursor*32, 0);


    al_flip_display();
    al_clear_to_color(al_map_rgb(255, 255, 255));
}

bool Menu::definirJogo(Controle& control,int& faseInicial, bool& doisJogadores, bool& done)
{

    if(!iniciando && !finalizando)
    {
        if(menuAtual==0)
        {
            if(control.getTeclas(UP)&&opcaoSelecionada>0)
            {
                opcaoSelecionada--;
                audioMenu.tocaAudio("CURSOR_MUDAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
            }

            if(control.getTeclas(DOWN)&&opcaoSelecionada<3)
            {
                opcaoSelecionada++;
                audioMenu.tocaAudio("CURSOR_MUDAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
            }

            if(control.getTeclas(X))
            {
                if(opcaoSelecionada==0)
                {
                    menuAtual=1;
                    opcaoSelecionada=0;
                    audioMenu.tocaAudio("CURSOR_SELECIONAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
                }
                else if(opcaoSelecionada==1)
                {
                    menuAtual=5;
                    opcaoSelecionada=0;
                    audioMenu.tocaAudio("CURSOR_SELECIONAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
                }
                else if(opcaoSelecionada==2)
                {
                    opcaoSelecionada = -10;
                    nomeRanking = ptrCarSav->getNomeRanking();
                    scoreRanking = ptrCarSav->getScoreRanking();
                    menuAtual = 7;
                    audioMenu.tocaAudio("CURSOR_SELECIONAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
                }
                else if(opcaoSelecionada==3)
                {
                    finalizando=true;
                    done = true;
                    audioMenu.tocaAudio("CURSOR_INICIAR_JOGO", 1, ALLEGRO_PLAYMODE_ONCE,0);
                    audioMenu.fadeTo("MUSICA_MENU",-0.005,0);
                }
            }
        }
        else if(menuAtual==1)
        {
            if(control.getTeclas(UP)&&opcaoSelecionada>0)
            {
                opcaoSelecionada--;
                audioMenu.tocaAudio("CURSOR_MUDAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
            }
            if(control.getTeclas(DOWN)&&opcaoSelecionada<2)
            {
                opcaoSelecionada++;
                audioMenu.tocaAudio("CURSOR_MUDAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
            }
            if(control.getTeclas(X))
            {
                ptrCarSav->setSave(opcaoSelecionada);
                if(ptrCarSav->saveCarregado(opcaoSelecionada))
                    menuAtual=3;
                else
                    menuAtual=2;
                opcaoSelecionada=0;
                audioMenu.tocaAudio("CURSOR_SELECIONAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
            }
            else if(control.getTeclas(C))
            {
                menuAtual=0;
                opcaoSelecionada=0;
                audioMenu.tocaAudio("CURSOR_VOLTAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
            }
        }
        else if(menuAtual==2)
        {
            if(control.getTeclas(UP)&&opcaoSelecionada>0)
            {
                opcaoSelecionada--;
                audioMenu.tocaAudio("CURSOR_MUDAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
            }
            if(control.getTeclas(DOWN)&&opcaoSelecionada<1)
            {
                opcaoSelecionada++;
                audioMenu.tocaAudio("CURSOR_MUDAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
            }

            if(control.getTeclas(X))
            {
                doisJogadores=opcaoSelecionada;
                nomeRanking = ptrCarSav->getNomeRanking();
                menuAtual = 8;
                opcaoSelecionada = -10;
                teclaPressionada = false;
                audioMenu.tocaAudio("CURSOR_SELECIONAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
                audioMenu.fadeTo("MUSICA_MENU",-0.005,0);
            }
            else if(control.getTeclas(C))
            {
                menuAtual=1;
                opcaoSelecionada=0;
                audioMenu.tocaAudio("CURSOR_VOLTAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
            }
        }
        else if(menuAtual==3)
        {
            if(control.getTeclas(UP)&&opcaoSelecionada>0)
            {
                opcaoSelecionada--;
                audioMenu.tocaAudio("CURSOR_MUDAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
            }
            if(control.getTeclas(DOWN)&&opcaoSelecionada<1)
            {
                opcaoSelecionada++;
                audioMenu.tocaAudio("CURSOR_MUDAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
            }
            if(control.getTeclas(X))
            {
                if(opcaoSelecionada==0)
                {
                    faseInicial = ptrCarSav->getFaseAtual(ptrCarSav->getSave());
                    if (faseInicial == 3)
                        faseInicial = 2;
                    finalizando=true;
                    audioMenu.tocaAudio("CURSOR_INICIAR_JOGO", 1, ALLEGRO_PLAYMODE_ONCE,0);
                    audioMenu.fadeTo("MUSICA_MENU",-0.005,0);
                }
                else if(opcaoSelecionada==1)
                {
                    checarFases(ptrCarSav->getSave());
                    menuAtual=4;
                    opcaoSelecionada=0;
                    audioMenu.tocaAudio("CURSOR_SELECIONAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
                }
            }
            else if(control.getTeclas(C))
            {
                menuAtual=1;
                opcaoSelecionada=0;
                audioMenu.tocaAudio("CURSOR_VOLTAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
            }
        }
        else if(menuAtual==4)
        {
            if(control.getTeclas(UP)&&opcaoSelecionada>0)
            {
                opcaoSelecionada--;
                audioMenu.tocaAudio("CURSOR_MUDAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
            }
            if(control.getTeclas(DOWN)&&opcaoSelecionada<2)
            {
                opcaoSelecionada++;
                audioMenu.tocaAudio("CURSOR_MUDAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
            }
            if(control.getTeclas(X))
            {
                if(permiteOpcao[opcaoSelecionada])
                {
                    faseInicial = opcaoSelecionada;
                    audioMenu.tocaAudio("CURSOR_INICIAR_JOGO", 1, ALLEGRO_PLAYMODE_ONCE,0);
                    audioMenu.fadeTo("MUSICA_MENU",-0.005,0);
                    finalizando=true;
                }
                else
                {
                    audioMenu.tocaAudio("CURSOR_NAO_PERMITIR", 1, ALLEGRO_PLAYMODE_ONCE,0);
                }
            }
            else if(control.getTeclas(C))
            {
                for(int i=0; i<3; i++)
                    permiteOpcao[i]=true;
                menuAtual=3;
                opcaoSelecionada=0;
                audioMenu.tocaAudio("CURSOR_VOLTAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
            }
        }
        else if(menuAtual==5)
        {
            if(control.getTeclas(X))
            {
                checarSaves();
                menuAtual=6;
                audioMenu.tocaAudio("CURSOR_SELECIONAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
            }
            else if(control.getTeclas(C))
            {
                menuAtual=0;
                audioMenu.tocaAudio("CURSOR_VOLTAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
            }
        }
        else if(menuAtual==6)
        {
            if(control.getTeclas(UP)&&opcaoSelecionada>0)
            {
                opcaoSelecionada--;
                audioMenu.tocaAudio("CURSOR_MUDAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
            }
            if(control.getTeclas(DOWN)&&opcaoSelecionada<2)
            {
                opcaoSelecionada++;
                audioMenu.tocaAudio("CURSOR_MUDAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
            }
            if(control.getTeclas(X))
            {
                if(permiteOpcao[opcaoSelecionada])
                {
                    ptrCarSav->deletaSave(opcaoSelecionada,true);
                    audioMenu.tocaAudio("CURSOR_SELECIONAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
                    checarSaves();
                }
                else
                {
                    audioMenu.tocaAudio("CURSOR_NAO_PERMITIR", 1, ALLEGRO_PLAYMODE_ONCE,0);
                }

            }
            else if(control.getTeclas(C))
            {
                for(int i=0; i<3; i++)
                    permiteOpcao[i]=true;
                menuAtual=5;
                opcaoSelecionada=0;
                audioMenu.tocaAudio("CURSOR_VOLTAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
            }
        }
        else if (menuAtual == 7)
        {
            if(control.getTeclas(C))
            {
                for(int i=0; i<4; i++)
                    permiteOpcao[i]=true;
                menuAtual=0;
                opcaoSelecionada=0;
                audioMenu.tocaAudio("CURSOR_VOLTAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
            }
        }
        else if (menuAtual == 8)
        {
            if (teclaPressionada)
            {
                char c;
                if (nome.size() < 20)
                {
                    if (tecla >= 1 && tecla <= 26)
                    {
                        c = (char)(64 + tecla);
                        nome += c;
                        if (!audioMenu.audioTocando("CURSOR_SELECIONAR"))
                            audioMenu.tocaAudio("CURSOR_SELECIONAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
                        else if (audioMenu.audioTocando("CURSOR_SELECIONAR"))
                        {
                            audioMenu.paraAudio("CURSOR_SELECIONAR");
                            audioMenu.tocaAudio("CURSOR_SELECIONAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
                        }
                    }
                    else if (tecla == 75)
                    {
                        nome += " ";
                        if (!audioMenu.audioTocando("CURSOR_SELECIONAR"))
                            audioMenu.tocaAudio("CURSOR_SELECIONAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
                        else if (audioMenu.audioTocando("CURSOR_SELECIONAR"))
                        {
                            audioMenu.paraAudio("CURSOR_SELECIONAR");
                            audioMenu.tocaAudio("CURSOR_SELECIONAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
                        }
                    }
                }
                if (tecla == 67)
                {
                    bool podeComecar = true;

                    Lista_Generica<string>::Iterador iteNome;
                    iteNome = nomeRanking->inicio();
                    for (int i = 0; i < nomeRanking->tamanho(); i++, iteNome++)
                        if (nome == (*iteNome))
                            podeComecar = false;
                    if (podeComecar)
                    {
                        faseInicial = 0;
                        finalizando=true;
                        ptrCarSav->setNomeSave(nome);
                        audioMenu.tocaAudio("CURSOR_INICIAR_JOGO", 1, ALLEGRO_PLAYMODE_ONCE,0);
                        audioMenu.fadeTo("MUSICA_MENU",-0.005,0);
                    }
                    else
                    {
                        audioMenu.tocaAudio("CURSOR_NAO_PERMITIR", 1, ALLEGRO_PLAYMODE_ONCE,0);
                    }

                }
                if (tecla == 63)
                {
                    if (nome.size())
                    {
                        nome.pop_back();
                        if (!audioMenu.audioTocando("CURSOR_VOLTAR"))
                            audioMenu.tocaAudio("CURSOR_VOLTAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
                        else if (audioMenu.audioTocando("CURSOR_VOLTAR"))
                        {
                            audioMenu.paraAudio("CURSOR_VOLTAR");
                            audioMenu.tocaAudio("CURSOR_VOLTAR", 1, ALLEGRO_PLAYMODE_ONCE,0);
                        }
                    }

                }
                teclaPressionada = false;
            }
        }

        control.setTeclas(UP, false);
        control.setTeclas(DOWN, false);
        control.setTeclas(X, false);
        control.setTeclas(C, false);
    }
    else if(finalizando && cor<=0)
    {
        resetarMenu();
        return true;
    }
    audioMenu.atualizaAudio();
    return false;
}

bool Menu::funcaoTransicao(bool& done)
{
    return false;
}
void Menu::checarSaves()
{
    for(int i=0; i<3; i++)
    {
        if(ptrCarSav->saveCarregado(i))
            permiteOpcao[i]=true;
        else
            permiteOpcao[i]=false;
    }
}
void Menu::checarFases(int s)
{
    int ultimaFase=ptrCarSav->getFaseAtual(s);
    for(int i=0; i<3; i++)
    {
        if(i<=ultimaFase)
            permiteOpcao[i]=true;
        else
            permiteOpcao[i]=false;
    }
}
void Menu::resetarMenu()
{
    for(int i=0; i<4; i++)
        permiteOpcao[i]=true;
    menuAtual=0;
    opcaoSelecionada=0;
    cor=0;
    iniciando=true;
    finalizando=false;
    teclaPressionada = false;
    audioMenu.atualizaAudio();
}
void Menu::setPtrCarSav(Carregar_Salvar * pcs)
{
    ptrCarSav = pcs;
}
void Menu::setTeclaPressionada(bool t)
{
    teclaPressionada = t;
}
void Menu::setTecla(int t)
{
    tecla = t;
}


