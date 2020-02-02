#include "Jogo.h"

Jogo::Jogo():
    pausa(&control),
    persistencia(this)
{
    state = -1;
    done = false;
    draw = false;
    saveCarregado = false;
    doisJogadores = true;
    largura=1200;
    altura=720;
    fase=0;
    checkpoint = 0;

    float *seno = new float[360];
    float *cosseno = new float[360];
    for (int i = 0; i < 360; i ++)
    {
        seno[i] = sin(i*(PI/180));
        cosseno[i] = cos(i*(PI/180));
    }
    Math::sen = seno;
    Math::coss = cosseno;


    if (!al_init())
        done = true;
    if (!al_init_image_addon())
        done = true;
    if (!al_init_primitives_addon())
        done = true;
    if (!al_init_acodec_addon())
        done = true;
    al_init_font_addon();
    if (!al_init_ttf_addon())
        done = true;
    if (!al_install_audio())
        done = true;
    if (!al_install_keyboard())
        done = true;
    if (!al_install_mouse())
        done = true;
    if (!al_install_joystick())
        done = true;

    if (!(display = al_create_display(largura, altura)))
        done = true;

    timer = al_create_timer(1.0 / FPS);

    eventQ = al_create_event_queue();
    font26 = al_load_font("moolbor_0.ttf", 26, 0);
    Entidade::font16 = al_load_font("LSANS.ttf", 16, 0);

    al_clear_to_color(al_map_rgb(0,0,0));
    al_draw_text(Entidade::font16, al_map_rgb(255,255,255), 0,720 - al_get_font_line_height(Entidade::font16),
                      0, "Carregando");
    al_flip_display();

    for (int i = 0; i < 3; i++)
        todasEnt[i].inicializaTimer();


    al_reserve_samples(40);
    menuPrincipal.inicializa();
    menuPrincipal.setPtrCarSav(&persistencia);
    pausa.inicializa();
    control.inicializaControle();

    al_register_event_source(eventQ, al_get_display_event_source(display));
    al_register_event_source(eventQ, al_get_timer_event_source(timer));
    al_register_event_source(eventQ, al_get_keyboard_event_source());
    al_register_event_source(eventQ, al_get_mouse_event_source());
    al_register_event_source(eventQ, al_get_joystick_event_source());


    if (!done)
        mudaEstado(state, MENU);

    al_start_timer(timer);

    principalLoopJogo();
}


Jogo::~Jogo()
{
    if (saveCarregado)
    {

        if ((fase > faseSave) || (fase == faseSave && checkpoint > checkpointSave))
        {
            persistencia.setScoreSave(todasEnt[fase].getScoreSave());
            persistencia.salvaLista(NULL, true);

        }
        persistencia.salvaRanking();
        for (int i = 0; i <3; i++)
            todasEnt[i].deletaListaEntidade();

    }


    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(eventQ);
    al_destroy_font(font26);
    al_destroy_font(Entidade::font16);

    delete [] Math::sen;
    delete [] Math::coss;

}


void Jogo::principalLoopJogo()
{
    while(!done)
    {
        if (state == LOADING)
            mudaEstado(state,PLAY);
        al_wait_for_event(eventQ, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER)
        {
            if (state == MENU)
                menu();
            else if (state == PLAY)
                atualiza();
            else if (state == PAUSE)
                menuPausa();
            else if (state == GAMEOVER)
                gameOver();

            if (al_get_num_joysticks() != 0)
                control.atualizaControle();

            draw = true;
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (state == MENU)
            {
                menuPrincipal.setTeclaPressionada(true);
                menuPrincipal.setTecla(ev.keyboard.keycode);
            }
            if (al_get_num_joysticks() == 0)
            {
                switch (ev.keyboard.keycode)
                {

                case ALLEGRO_KEY_UP:
                    control.setTeclas(UP, true);
                    break;
                case ALLEGRO_KEY_DOWN:
                    control.setTeclas(DOWN, true);
                    break;
                case ALLEGRO_KEY_RIGHT:
                    control.setTeclas(RIGHT, true);
                    break;
                case ALLEGRO_KEY_LEFT:
                    control.setTeclas(LEFT, true);
                    break;
                case ALLEGRO_KEY_Z:
                    control.setTeclas(Z, true);
                    break;
                case ALLEGRO_KEY_X:
                    control.setTeclas(X, true);
                    break;
                case ALLEGRO_KEY_C:
                    control.setTeclas(C, true);
                    break;
                case ALLEGRO_KEY_V:
                    control.setTeclas(V, true);
                    break;
                }
            }
            if (al_get_num_joysticks() == 0 || al_get_num_joysticks() == 1)
            {
                switch (ev.keyboard.keycode)
                {
                case ALLEGRO_KEY_I:
                    control.setTeclas(W, true);
                    break;
                case ALLEGRO_KEY_J:
                    control.setTeclas(A, true);
                    break;
                case ALLEGRO_KEY_K:
                    control.setTeclas(S, true);
                    break;
                case ALLEGRO_KEY_L:
                    control.setTeclas(D, true);
                    break;
                case ALLEGRO_KEY_A:
                    control.setTeclas(F, true);
                    break;
                case ALLEGRO_KEY_S:
                    control.setTeclas(G, true);
                    break;
                case ALLEGRO_KEY_D:
                    control.setTeclas(H, true);
                    break;
                case ALLEGRO_KEY_E:
                    control.setTeclas(J, true);
                    break;

                }
            }
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                    done = true;

        }
        else if (ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            if (al_get_num_joysticks() == 0)
            {
                switch (ev.keyboard.keycode)
                {
                case ALLEGRO_KEY_UP:
                    control.setTeclas(UP, false);
                    break;
                case ALLEGRO_KEY_DOWN:
                    control.setTeclas(DOWN, false);
                    break;
                case ALLEGRO_KEY_RIGHT:
                    control.setTeclas(RIGHT, false);
                    break;
                case ALLEGRO_KEY_LEFT:
                    control.setTeclas(LEFT, false);
                    break;
                case ALLEGRO_KEY_Z:
                    control.setTeclas(Z, false);
                    break;
                case ALLEGRO_KEY_X:
                    control.setTeclas(X, false);
                    break;
                case ALLEGRO_KEY_C:
                    control.setTeclas(C, false);
                    break;
                case ALLEGRO_KEY_V:
                    control.setTeclas(V, false);
                    break;
                }
            }
            if (al_get_num_joysticks() == 0 || al_get_num_joysticks() == 1)
            {
                switch (ev.keyboard.keycode)
                {
                case ALLEGRO_KEY_I:
                    control.setTeclas(W, false);
                    break;
                case ALLEGRO_KEY_J:
                    control.setTeclas(A, false);
                    break;
                case ALLEGRO_KEY_K:
                    control.setTeclas(S, false);
                    break;
                case ALLEGRO_KEY_L:
                    control.setTeclas(D, false);
                    break;
                case ALLEGRO_KEY_A:
                    control.setTeclas(F, false);
                    break;
                case ALLEGRO_KEY_S:
                    control.setTeclas(G, false);
                    break;
                case ALLEGRO_KEY_D:
                    control.setTeclas(H, false);
                    break;
                case ALLEGRO_KEY_E:
                    control.setTeclas(J, false);
                    break;
                }
            }
        }
        else if (ev.type == ALLEGRO_EVENT_JOYSTICK_CONFIGURATION)
        {
            control.reconfiguraControle();
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            done = true;
        if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            Mapa::mouseX = ev.mouse.x;
            Mapa::mouseY = ev.mouse.y;
        }

        if (draw && al_is_event_queue_empty(eventQ) && !done)
        {
            draw = false;
            if (state == MENU)
                desenhaMenu();
            else if (state == PLAY)
                desenhaJogo();
            else if (state == PAUSE)
                desenhaMenuPausa();
            else if (state == GAMEOVER)
                desenhaGameOver();
        }

    }
}

void Jogo::menu()
{
    if (menuPrincipal.definirJogo(control, fase, doisJogadores, done))
        if (!done)
            mudaEstado(state, LOADING);
}

void Jogo::desenhaMenu()
{
    menuPrincipal.desenhaMenu();
}


void Jogo::atualiza()
{
    Sprite::setContTempo(al_get_timer_count(timer));
    todasEnt[fase].atualiza();
    todasEnt[fase].colisoesChao();
    todasEnt[fase].colisoesEnt();
}

void Jogo::desenhaJogo()
{
    todasEnt[fase].desenha();
    if (control.getTeclas(V))
        mudaEstado(state, PAUSE);
    al_flip_display();
    al_clear_to_color(al_map_rgb(0, 0, 0));
}

void Jogo::menuPausa()
{
    pausa.atualiza();
    if (pausa.getMudarEstado())
    {
        if (pausa.getEstado() != FECHA)
            mudaEstado(state, pausa.getEstado());
        else
            done = true;

    }
}

void Jogo::desenhaMenuPausa()
{
    todasEnt[fase].desenha();
    pausa.desenha();
    al_flip_display();
    al_clear_to_color(al_map_rgb(0, 0, 0));
}


void Jogo::gameOver()
{

}

void Jogo::desenhaGameOver()
{

}


void Jogo::mudaEstado(int &oldState, int newState)
{

    if (oldState == MENU)
    {
        saveCarregado = true;
        if(persistencia.saveCarregado(persistencia.getSave()))
        {
            faseSave = persistencia.getFaseAtual(persistencia.getSave());
            checkpointSave = persistencia.getCheckpointAtual();

            if (faseSave != fase)
            {
                checkpoint = 0;
            }
            else if ( faseSave == fase)
            {
                checkpoint = checkpointSave;
                todasEnt[fase].setScore(persistencia.getScoreSave());
            }
            doisJogadores = persistencia.getDoisJogadores();
        }
        else
        {
            fase = faseSave = 0;
            checkpoint = checkpointSave = 0;
            persistencia.inicializaSave();
            persistencia.setScoreSave(0);
        }
    }
    else if (oldState == PLAY)
    {

    }
    else if (oldState == PAUSE)
    {
        if (newState == MENU)
        {
            todasEnt[fase].paraMusica();
        }
        if (newState == PLAY)
        {
            todasEnt[fase].despausaEntidades();
        }
        al_set_timer_count(timer, tempoPausa);
        Sprite::setContTempo(tempoPausa);
    }
    else if (oldState == GAMEOVER)
    {

    }

    if (newState == MENU)
    {
        saveCarregado = false;
        persistencia.reseta();
        if (oldState == PAUSE)
        {
            if ((fase > faseSave) || (fase == faseSave && checkpoint > checkpointSave))
            {
                persistencia.setScoreSave(todasEnt[fase].getScoreSave());
                persistencia.salvaLista(NULL, true);
            }
            for (int i = 0; i < 3; i++)
            {
                todasEnt[i].deletaListaEntidade();
            }
            persistencia.salvaRanking();
        }
        else if (oldState == PLAY)
        {
            persistencia.setScoreSave(todasEnt[fase-1].getScoreSave());
            persistencia.salvaLista(NULL, true);
            persistencia.limpaLista();
            persistencia.salvaRanking();
            for (int i = 0; i < 3; i++)
            {
                todasEnt[i].deletaListaEntidade();
            }
        }
        persistencia.carregaRanking();
        menuPrincipal.resetarMenu();
    }
    else if (newState == LOADING)
    {
        al_clear_to_color(al_map_rgb(0,0,0));
        al_draw_text(Entidade::font16, al_map_rgb(255,255,255), 0,720 - al_get_font_line_height(Entidade::font16),
                      0, "Carregando");
        al_flip_display();
        Gerenciador_Fases::criaFase( this, todasEnt, doisJogadores, &control);
        if (fase == faseSave && checkpoint >= checkpointSave)
        {
            persistencia.carregaLista(todasEnt[fase].getListGen(), true);
            todasEnt[fase].setScore(persistencia.getScoreSave());
        }

        if (checkpoint != 0)
            Gerenciador_Fases::atualizaFase(fase, checkpoint);

    }
    else if (newState == PLAY)
    {
        if (oldState == PAUSE)
        {
            al_set_timer_count(timer, tempoPausa);
            Sprite::setContTempo(tempoPausa);
        }


    }
    else if (newState == PAUSE)
    {
        todasEnt[fase].pausaEntidades();
        tempoPausa = al_get_timer_count(timer);
        pausa.resetaPausa();
    }
    else if (newState == GAMEOVER)
    {

    }
    al_flush_event_queue(eventQ);
    draw = false;
    oldState = newState;
}
void Jogo::salvaPosicao(Lista_Generica<Entidade*> * listE)
{
    persistencia.salvaLista(listE, false);
    persistencia.setScoreSave(todasEnt[fase].getScoreSave());
}
void Jogo::carregaPosicao(Lista_Generica<Entidade*> * listE)
{
    persistencia.carregaLista(listE, false);
}
void Jogo::setFase(int f)
{
    fase = f;
}
int Jogo::getFase()
{
    return fase;
}
bool Jogo::getDoisJog()
{
    return doisJogadores;
}
void Jogo::setCheckpoint(int c)
{
    checkpoint = c;
}
int Jogo::getCheckpoint()
{
    return checkpoint;
}
void Jogo::resetaSave(int f, int c)
{
    int faseI = fase;
    fase = f;
    checkpoint = c;
    if (fase != 3)
        todasEnt[fase].setScore(todasEnt[faseI].getScore());
    if (fase > faseSave)
    {
        persistencia.setScoreSave(todasEnt[faseI].getScore());
        persistencia.limpaLista();
    }
    al_set_timer_count(timer, 0);
    Sprite::setContTempo(0);
    if (fase == 3)
    {
        checkpointSave = checkpoint = 0;
        mudaEstado(state, MENU);
    }
}
ALLEGRO_TIMER * Jogo::getTimer()
{
    return timer;
}
