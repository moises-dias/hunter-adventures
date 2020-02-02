#include "Carregar_Salvar.h"
#include "Jogo.h"

Carregar_Salvar::Carregar_Salvar()
{

}
Carregar_Salvar::Carregar_Salvar(Jogo* pJo)
{
    ptrJogo = pJo;
    saveAtual = 0;
    linha = new char[150];
}
Carregar_Salvar::~Carregar_Salvar()
{
    saveI[0].close();
    saveI[1].close();
    saveI[2].close();

    rankingI.close();
    delete [] linha;
}
void Carregar_Salvar::reseta()
{
    if (saveI[0].is_open())
        saveI[0].close();
    if (saveI[1].is_open())
        saveI[1].close();
    if (saveI[2].is_open())
        saveI[2].close();
    if (rankingI.is_open())
        rankingI.close();

    if (!saveI[0].is_open())
        saveI[0].open("Dados/Save/save0.txt");
    if (!saveI[1].is_open())
        saveI[1].open("Dados/Save/save1.txt");
    if (!saveI[2].is_open())
        saveI[2].open("Dados/Save/save2.txt");
    if (!rankingI.is_open())
        rankingI.open("Dados/Save/ranking.txt");

}
void Carregar_Salvar::salvaLista(Lista_Generica<Entidade*> * listE, bool persistir)
{
    if (!persistir)
    {
        if (posIni.tamanho() != 0)
        {
            posIni.limpar();
            indIni.limpar();
        }
        convertToListVec(listE);
    }
    else
    {
        Lista_Generica<Vetor_R2>::Iterador iteP;
        iteP = posIni.inicio();
        Lista_Generica<int>::Iterador iteI;
        iteI = indIni.inicio();
        deletaSave(saveAtual, false);
        char *num = "0123456789";
        string s1 = "Dados/Save/save";
        s1 += num[saveAtual];
        s1 += ".txt";
        saveO.open(s1.c_str());
        saveO.seekp(0);
        /**[0]: já salvou? - [1]:fase - [2]:doisJog - [3]:checkpoint # score # nome do jogador '#'**/
        saveO << 1 << ptrJogo->getFase() << ptrJogo->getDoisJog() << ptrJogo->getCheckpoint() << '#' << scoreSave << '#' << nomeS << '#' <<endl;
        /**indice # posicaoX # posicaoY #**/
        for (int i = 0; i < posIni.tamanho(); i++, iteP++, iteI++)
        {
            saveO << (*iteI) << '#' << ((int)(*iteP).getX()) << '#' << ((int)(*iteP).getY()) << '#' << endl;
        }
        saveO.close();

    }

}
void Carregar_Salvar::carregaLista(Lista_Generica<Entidade*> * listE, bool salvo)
{
    if (!salvo)
    {
        convertToListEnt(listE);
    }
    else
    {
        saveI[saveAtual].seekg(0);
        saveI[saveAtual].getline(linha, 150);
        while(saveI[saveAtual].getline(linha,150))
        {
            int i, j = 0;
            int index = -1, posX = -1, posY = -1;
            for (i = 0; linha[i] != '#'; i++);
            index = charToIntRanged(linha, j, i-1);
            i++;
            j = i;
            for (; linha[i] != '#'; i++);
            posX = charToIntRanged(linha, j, i-1);
            i++;
            j = i;
            for (; linha[i] != '#'; i++);
            posY = charToIntRanged(linha, j, i-1);
            if (index != -1 && posX != -1 && posY != -1)
            {
                indIni.inclui_frente(index);
                posIni.inclui_frente(Vetor_R2::vetor(posX, posY));
            }
        }
        convertToListEnt(listE);
    }


}

bool Carregar_Salvar::saveCarregado(int s)
{
    saveI[s].seekg(0);
    saveI[s].getline(linha,150);
    if (linha[0] ==  '1')
        return true;
    return false;

}
void Carregar_Salvar::deletaSave(int s, bool reseta)
{
    if (saveO.is_open())
        saveO.close();
    char *num = "0123456789";
    string s1 = "Dados/Save/save";
    s1 += num[s];
    s1 += ".txt";
    saveO.open(s1.c_str());
    if (reseta)
    {
        saveO << 0;
    }
    saveO.close();

}
void Carregar_Salvar::limpaLista()
{
    if (saveO.is_open())
        saveO.close();
    char *num = "0123456789";
    string s1 = "Dados/Save/save";
    s1 += num[saveAtual];
    s1 += ".txt";
    saveO.open(s1.c_str());
    saveO << 1 << ptrJogo->getFase() << ptrJogo->getDoisJog() << ptrJogo->getCheckpoint() << '#' << scoreSave << '#' << nomeS << '#';
    saveO.close();
}
void Carregar_Salvar::setSave(int s)
{
    saveAtual = s;
    saveI[saveAtual].seekg(0);
    saveI[saveAtual].getline(linha, 150);
    int i, f;
    for (f = 0; linha[f] != '#'; f++);
    i = f + 1;
    for (f = i; linha[f] != '#'; f++);
    scoreSave = charToIntRanged(linha, i, f-1);
    i = f + 1;
    string s1;
    for (f = i; linha[f] != '#'; f++)
        s1 += linha[f];
    nomeS = s1;
}
int Carregar_Salvar::getFaseAtual(int s)
{
    saveI[s].seekg(0);
    saveI[s].getline(linha, 150);
    if (linha[0] == '1')
        return charToInt(linha[1]);
    return 0;
}
int Carregar_Salvar::charToIntRanged(char* c, int i, int f)
{
    int n = 0;
    int pow = 1;
    for (int j = f; j >= i; j--)
    {
        n += charToInt(c[j])*pow;
        pow *= 10;
    }
    return n;
}
int Carregar_Salvar::charToInt(char c)
{
    return (int)c - 48;
}
void Carregar_Salvar::convertToListEnt(Lista_Generica<Entidade*> * listE)
{
    Lista_Generica<Entidade*>::Iterador iteE;
    iteE = listE->inicio();
    Lista_Generica<Vetor_R2>::Iterador iteP;
    iteP = posIni.inicio();
    Lista_Generica<int>::Iterador iteI;
    iteI = indIni.inicio();

    Inimigo* ptrI = NULL;
    iteE++;
    Entidade* ptrJ =(*iteE);
    iteE++;

    int j = posIni.tamanho();
    if (j != 0)
    {
        for (int i = 2; i < listE->tamanho(); i++, iteE++)
        {

            if (i == (*iteI))
            {
                ptrI = static_cast<Inimigo*> (*iteE);
                ptrI->resetaEntidade();
                if ((*iteP).getX() > ptrJ->getPos().getEspaco().getX() - 200 &&
                        (*iteP).getX() < ptrJ->getPos().getEspaco().getX() + 200 &&
                        (*iteP).getY() > ptrJ->getPos().getEspaco().getY() - 200 &&
                        (*iteP).getY() < ptrJ->getPos().getEspaco().getY() + 200)
                    ptrI->getPos().getEspaco().setXY(ptrJ->getPos().getEspaco().getX() - 200, (*iteP).getY());
                else
                    ptrI->getPos().setEspaco((*iteP));
                ptrI->setFuncionando(true);
                iteI++;
                iteP++;
                j--;
                if (!j)
                    break;
            }
        }
    }

}
void Carregar_Salvar::convertToListVec(Lista_Generica<Entidade*> * listE)
{
    Lista_Generica<Entidade*>::Iterador iteE;
    iteE = listE->inicio();

    for (int i = 0; i < listE->tamanho(); i++, iteE++)
    {
        if ((*iteE)->getID() == 4 && (*iteE)->getFuncionando())
        {
            posIni.inclui_frente((*iteE)->getPos().getEspaco());
            indIni.inclui_frente(i);
        }
    }
}
int Carregar_Salvar::getSave()
{
    return saveAtual;
}
int Carregar_Salvar::getCheckpointAtual()
{
    saveI[saveAtual].seekg(0);
    saveI[saveAtual].getline(linha, 150);
    if (linha[0] == '1')
        return charToInt(linha[3]);
    return 0;
}
bool Carregar_Salvar::getDoisJogadores()
{
    saveI[saveAtual].seekg(0);
    saveI[saveAtual].getline(linha, 150);
    if (linha[0] == '1')
        return charToInt(linha[2]);
    return 0;
}
void Carregar_Salvar::inicializaSave()
{
    char *num = "0123456789";
    string s1 = "Dados/Save/save";
    s1 += num[saveAtual];
    s1 += ".txt";
    saveO.open(s1.c_str());
    saveO << 1 << 0 << ptrJogo->getDoisJog() << 0 << '#' << 0 << '#' << nomeS << '#' << endl;
    saveO.close();
}
void Carregar_Salvar::carregaRanking()
{
    if (nomeScore.tamanho())
        nomeScore.limpar();
    if (score.tamanho())
        score.limpar();
    rankingI.seekg(0);
    while(rankingI.getline(linha, 150))
    {
        int i = 0, j = 0;
        string s1;
        for(i = j; linha[i] != '#'; i++)
            s1 += linha[i];
        nomeScore.inclui_frente(s1);
        j = i + 1;
        for (i = j; linha[i] != '#'; i++);
        i--;
        score.inclui_frente(charToIntRanged(linha,j,i));
    }
}
void Carregar_Salvar::salvaRanking()
{
    Lista_Generica<int>::Iterador iteR, iteAux;
    iteR = score.inicio();
    Lista_Generica<string>::Iterador iteN;
    iteN = nomeScore.inicio();
    if (score.tamanho() == 0)
    {
        score.inclui_frente(scoreSave);
        nomeScore.inclui_frente(nomeS);
    }
    else
    {
        for (int i = 0; i < score.tamanho(); i++, iteR++, iteN++)
        {
            if (nomeS == (*iteN))
            {
                if (scoreSave > (*iteR))
                {
                    iteR.getElemento()->setInfo(scoreSave);
                }
                break;
            }
            if (i < 9)
            {
                if (i != score.tamanho() - 1)
                {
                    iteAux = iteR.getElemento();
                    iteAux++;
                    if (scoreSave > (*iteR))
                    {
                        score.inclui_traz(scoreSave);
                        nomeScore.inclui_traz(nomeS);
                        break;
                    }
                    else if (scoreSave < (*iteR) && scoreSave >= (*iteAux))
                    {
                        score.adiciona_apos(i, scoreSave);
                        nomeScore.adiciona_apos(i, nomeS);
                        break;
                    }
                }
                else
                {
                    score.inclui_frente(scoreSave);
                    nomeScore.inclui_frente(nomeS);
                    break;
                }
            }
            else if (i == 9)
            {
                if (scoreSave > (*iteR))
                {
                    iteR.getElemento()->setInfo(scoreSave);
                    iteN.getElemento()->setInfo(nomeS);
                    break;
                }
            }
            else
            {
                score.retira_frente();
                nomeScore.retira_frente();
                i--;
            }
        }
    }

    while (score.tamanho() > 10)
    {
        score.retira_frente();
        nomeScore.retira_frente();
    }
    rankingO.open("Dados/Save/ranking.txt");
    while(nomeScore.tamanho())
        rankingO << nomeScore.retira_traz() <<  '#' << score.retira_traz() << '#' << endl;
    rankingO.close();

}
void Carregar_Salvar::setScoreSave(int s)
{
    scoreSave = s;
}
int Carregar_Salvar::getScoreSave()
{
    return scoreSave;
}
Lista_Generica<string>* Carregar_Salvar::getNomeRanking()
{
    return &nomeScore;
}
Lista_Generica<int>* Carregar_Salvar::getScoreRanking()
{
    return &score;
}
void Carregar_Salvar::setNomeSave(string nome)
{
    nomeS = nome;
}
