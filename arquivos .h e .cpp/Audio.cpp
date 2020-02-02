#include "Audio.h"

Audio::Audio()
{

}
Audio::~Audio()
{
    while(audioInst.tamanho())
        al_destroy_sample_instance(audioInst.retira_frente());
    while(audio.tamanho())
        al_destroy_sample(audio.retira_frente());

}
void Audio::inicializaAudio(string * nomes, int n)
{
    string sAux1, sAux2 = "Dados/Audio/", sAux3 = ".ogg";
    for (int i = 0; i < n; i++)
    {
        nomeAudio.inclui_frente(nomes[i]);

        sAux1 = sAux2 + nomes[i] + sAux3;
        audio.inclui_frente(al_load_sample(sAux1.c_str()));
    }
}
void Audio::atualizaAudio()
{
    Lista_Generica<float>::Iterador iteAlt;
    iteAlt = audioInfo[ALTURA].inicio();
    Lista_Generica<float>::Iterador iteVde;
    iteVde = audioInfo[VEL_MUDANCA].inicio();
    Lista_Generica<float>::Iterador iteMod;
    iteMod = audioInfo[MODO].inicio();
    Lista_Generica<float>::Iterador iteAlf;
    iteAlf = audioInfo[ALTURA_F].inicio();
    Lista_Generica<ALLEGRO_SAMPLE_INSTANCE*>::Iterador iteInst;
    iteInst = audioInst.inicio();
    for (int i = 0; i < audioInst.tamanho(); i++)
    {
        if (al_get_sample_instance_playing((*iteInst)))
        {
            if ((*iteVde) != 0)
            {
                if ((*iteVde) > 0 && iteAlt.getElemento()->getInfo() + (*iteVde) < (*iteAlf))
                    iteAlt.getElemento()->setInfo(iteAlt.getElemento()->getInfo() + (*iteVde));
                else if ((*iteVde) > 0 && iteAlt.getElemento()->getInfo() + (*iteVde) > (*iteAlf))
                    iteAlt.getElemento()->setInfo((*iteAlf));
                else if ((*iteVde) < 0 && iteAlt.getElemento()->getInfo() + (*iteVde) > (*iteAlf))
                    iteAlt.getElemento()->setInfo(iteAlt.getElemento()->getInfo() + (*iteVde));
                else if ((*iteVde) < 0 && iteAlt.getElemento()->getInfo() + (*iteVde) < (*iteAlf))
                    iteAlt.getElemento()->setInfo((*iteAlf));

                if ((*iteAlt) > 0)
                    al_set_sample_instance_gain((*iteInst), (*iteAlt));
                else
                {
                    al_stop_sample_instance((*iteInst));
                    al_destroy_sample_instance(audioInst.removerEle(i));
                    nomeInst.removerEle(i);
                    audioInfo[ALTURA].removerEle(i);
                    audioInfo[MODO].removerEle(i);
                    audioInfo[VEL_MUDANCA].removerEle(i);
                    audioInfo[ALTURA_F].removerEle(i);
                    if(audioInst.tamanho()==0)
                        break;
                    if (i == 0)
                    {
                        iteAlt = audioInfo[ALTURA].inicio();
                        iteVde = audioInfo[VEL_MUDANCA].inicio();
                        iteMod = audioInfo[MODO].inicio();
                        iteAlf = audioInfo[ALTURA_F].inicio();
                        iteInst = audioInst.inicio();
                    }
                    else if (i > 0 && i < audioInst.tamanho() - 1)
                    {
                        iteAlt = audioInfo[ALTURA].retornaElemento(i - 1);
                        iteVde = audioInfo[VEL_MUDANCA].retornaElemento(i - 1);
                        iteMod = audioInfo[MODO].retornaElemento(i - 1);
                        iteAlf = audioInfo[ALTURA_F].retornaElemento(i - 1);
                        iteInst = audioInst.retornaElemento(i - 1);
                    }
                    else if (i == audioInst.tamanho() - 1)
                        break;
                    i--;

                }
            }
        }
        else
        {
            al_destroy_sample_instance(audioInst.removerEle(i));
            nomeInst.removerEle(i);
            audioInfo[ALTURA].removerEle(i);
            audioInfo[MODO].removerEle(i);
            audioInfo[VEL_MUDANCA].removerEle(i);
            audioInfo[ALTURA_F].removerEle(i);
            if(audioInst.tamanho()==0)
                break;
            if (i == 0)
            {
                iteAlt = audioInfo[ALTURA].inicio();
                iteVde = audioInfo[VEL_MUDANCA].inicio();
                iteMod = audioInfo[MODO].inicio();
                iteAlf = audioInfo[ALTURA_F].inicio();
                iteInst = audioInst.inicio();
            }
            else if (i > 0 && i < audioInst.tamanho() - 1)
            {
                iteAlt = audioInfo[ALTURA].retornaElemento(i - 1);
                iteVde = audioInfo[VEL_MUDANCA].retornaElemento(i - 1);
                iteMod = audioInfo[MODO].retornaElemento(i - 1);
                iteAlf = audioInfo[ALTURA_F].retornaElemento(i - 1);
                iteInst = audioInst.retornaElemento(i - 1);
            }
            else if (i == audioInst.tamanho() - 1)
                break;
            i--;
        }
        if (i != -1)
        {
            iteAlt++;
            iteVde++;
            iteInst++;
            iteMod++;
            iteAlf++;
        }

    }
}
void Audio::tocaAudio(string nome,float altura, ALLEGRO_PLAYMODE modo, float velMudanca)
{
    Lista_Generica<string>::Iterador iteNome;
    Lista_Generica<ALLEGRO_SAMPLE*>::Iterador iteSam;
    iteNome = nomeAudio.inicio();
    iteSam = audio.inicio();

    for (int i = 0; i < nomeAudio.tamanho(); i++, iteNome++, iteSam++)
    {
        if ((*iteNome) == nome)
        {
            ALLEGRO_SAMPLE_INSTANCE* inst = al_create_sample_instance((*iteSam));
            al_attach_sample_instance_to_mixer(inst, al_get_default_mixer());
            al_set_sample_instance_gain(inst, altura);
            al_set_sample_instance_playmode(inst, modo);
            audioInst.inclui_frente(inst);
            nomeInst.inclui_frente(nome);
            audioInfo[ALTURA].inclui_frente(altura);
            audioInfo[ALTURA_F].inclui_frente(altura);
            switch (modo)
            {
            case ALLEGRO_PLAYMODE_LOOP:
                audioInfo[MODO].inclui_frente(LOOP);
                break;
            case ALLEGRO_PLAYMODE_ONCE:
                audioInfo[MODO].inclui_frente(ONCE);
                break;
            }
            audioInfo[VEL_MUDANCA].inclui_frente(velMudanca);
            al_play_sample_instance(inst);
            break;

        }
    }


}
void Audio::paraAudio(string nome)
{
    Lista_Generica<ALLEGRO_SAMPLE_INSTANCE*>::Iterador ite;
    ite = audioInst.inicio();
    Lista_Generica<string>::Iterador iteNome;
    iteNome = nomeInst.inicio();
    for (int i = 0; i < audioInst.tamanho(); i++, ite++, iteNome++)
    {
        if ((*iteNome) == nome)
        {
            al_stop_sample_instance((*ite));
            al_destroy_sample_instance(audioInst.removerEle(i));
            nomeInst.removerEle(i);
            audioInfo[ALTURA].removerEle(i);
            audioInfo[MODO].removerEle(i);
            audioInfo[VEL_MUDANCA].removerEle(i);
            audioInfo[ALTURA_F].removerEle(i);
            break;
        }
    }
}
void Audio::fadeTo(string nome, float vFade, float alturaFinal)
{
    Lista_Generica<float>::Iterador ite;
    ite = audioInfo[VEL_MUDANCA].inicio();
    Lista_Generica<float>::Iterador iteA;
    iteA = audioInfo[ALTURA_F].inicio();
    Lista_Generica<string>::Iterador iteNome;
    iteNome = nomeInst.inicio();
    for (int i = 0; i < audioInst.tamanho(); i++, ite++, iteNome++, iteA++)
    {
        if ((*iteNome) == nome)
        {
            iteA.getElemento()->setInfo(alturaFinal);
            ite.getElemento()->setInfo(vFade);
            break;
        }
    }
}
bool Audio::audioTocando(string nome)
{
    Lista_Generica<string>::Iterador ite;
    ite = nomeInst.inicio();
    for (int i = 0; i < nomeInst.tamanho(); i++, ite++)
        if ((*ite) == nome)
            return true;
    return false;
}
