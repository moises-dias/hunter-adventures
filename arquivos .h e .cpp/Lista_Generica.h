#ifndef LISTA_GENERICA_H
#define LISTA_GENERICA_H

#include "CEI.h"

template <class E>
class Lista_Generica
{
public:

    class Elemento
    {
    public:
        Elemento()
        {
            proximo = anterior = NULL;
            info = NULL;
        }
        Elemento(E novo)
        {
            proximo = anterior = NULL;
            info = novo;
        }
        ~Elemento()
        {
        }

        void setProximo(Elemento * p)
        {
            proximo = p;
        }
        Elemento* getProximo()
        {
            return proximo;
        }

        void setAnterior(Elemento * a)
        {
            anterior = a;
        }
        Elemento* getAnterior()
        {
            return anterior;
        }


        void setInfo(E i)
        {
            info = i;
        }
        E getInfo()
        {
            return info;
        }

    private:
        Elemento * proximo;
        Elemento * anterior;
        E info;
    };


    class Iterador
    {
    public:
        Iterador() {}
        ~Iterador() {}

        void setPtrEle(Elemento * e)
        {
            ptrEle = e;
        }
        Elemento* getElemento()
        {
            return ptrEle;
        }

        Iterador operator++(int)
        {
            ptrEle = ptrEle->getProximo();
            return *this;
        }
        Iterador& operator+=(int n)
        {
            for (int i = 0; i < n; (*this)++, i++);
            return *this;
        }
        Iterador operator--(int)
        {
            ptrEle = ptrEle->getAnterior();
            return *this;
        }
        Iterador& operator-=(int n)
        {
            for (int i = 0; i < n; (*this)--, i++);
            return *this;
        }
        bool operator==(Elemento* rh)
        {
            if (ptrEle != NULL)
                return ptrEle->getInfo == rh->getInfo;
            else if (ptrEle == NULL && rh == NULL)
                return true;
            return false;
        }
        bool operator!=(Elemento* rh)
        {
            if (ptrEle != NULL)
                return ptrEle->getInfo() != rh->getInfo();
            else if (ptrEle == NULL && rh == NULL)
                return false;
            return true;
        }

        Iterador operator=(Elemento * igual)
        {
            (*this).setPtrEle(igual);
            return (*this);
        }
        E operator*()
        {
            return ptrEle->getInfo();

        }

    private:
        Elemento *ptrEle;
    };

    Lista_Generica();
    ~Lista_Generica();
    void inclui_frente(E ele);
    void inclui_traz(E ele);
    void adiciona_apos(int i, E ele);
    E retira_frente();
    E retira_traz();
    int tamanho(); // retorna o tamanho da lista
    E* limpar(); // deleta todos os elementos da lista
    E removerEle(int i); // Emove elemento de indice i
    Elemento* inicio();
    Elemento* fim();
    Elemento* retornaElemento(int i);
    void deletaElemento(Elemento* ele);

    E operator[](int i); // Retorna a info do elemento de indice i
private:
    Elemento  *topo;
    Elemento  *base;
    int tam;

};

template <class E>
Lista_Generica<E>::Lista_Generica()
{
    topo = NULL;
    base = NULL;
    tam = 0;
}

template <class E>
Lista_Generica<E>::~Lista_Generica()
{
}

template <class E>
void Lista_Generica<E>::inclui_frente(E ele)
{
    if (topo == NULL)
    {
        topo = new Elemento (ele);
        base = topo;
    }
    else
    {
        base->setProximo(new Elemento (ele));
        base->getProximo()->setAnterior(base);
        base = base->getProximo();
    }
    tam ++;
}
template <class E>
void Lista_Generica<E>::inclui_traz(E ele)
{
    if (topo == NULL)
    {
        topo = new Elemento (ele);
        base = topo;
    }
    else
    {
        topo->setAnterior(new Elemento (ele));
        topo->getAnterior()->setProximo(topo);
        topo = topo->getAnterior();
    }
    tam ++;
}
template <class E>
void Lista_Generica<E>::adiciona_apos(int i, E ele)
{
    Elemento * ele1 = retornaElemento(i), *ele2;
    Elemento * aux = new Elemento(ele);
    if (ele1->getProximo() != NULL)
    {
        ele2 = ele1->getProximo();
        ele1->setProximo(aux);
        ele2->setAnterior(aux);
        aux->setAnterior(ele1);
        aux->setProximo(ele2);
    }
    else
    {
        ele1->setProximo(aux);
        aux->setAnterior(ele1);
    }

}
template <class E>
E Lista_Generica<E>::retira_frente()
{
    E eleAux;
    Elemento* aux;
    eleAux = base->getInfo();
    aux = base;
    if (base != topo)
    {
        base = base->getAnterior();
        base->setProximo(NULL);
    }
    else
    {
        base = topo = NULL;
    }
    delete aux;
    tam --;
    return eleAux;
}
template <class E>
E Lista_Generica<E>::retira_traz()
{
    E eleAux;
    Elemento* aux;
    eleAux = topo->getInfo();
    aux = topo;
    if (topo != base)
    {
        topo = topo->getProximo();
        topo->setAnterior(NULL);
    }
    else
    {
        topo = base = NULL;
    }
    delete aux;
    tam --;
    return eleAux;
}

template <class E>
int Lista_Generica<E>::tamanho()
{
    return tam;
}

template <class E>
E* Lista_Generica<E>::limpar()
{
    Elemento* aux;
    E *infoDel;
    infoDel = new E [tamanho()];
    tam = 0;
    int i = 0;
    if (topo != NULL)
    {
        aux = topo;
        infoDel[i] = aux->getInfo();
        while(topo != base)
        {
            topo = topo->getProximo();
            i++;
            infoDel[i] = topo->getInfo();
            delete aux;
            aux = topo;
        }
        delete topo;
        topo = base = NULL;
        return infoDel;
    }
    return 0;
}
template <class E>
E Lista_Generica<E>::operator[](int i)
{
    Elemento* aux;
    if (topo != NULL)
    {
        aux = topo;
        for(int j = 0; j < tamanho(); j++)
        {
            if (j == i)
                return aux->getInfo();
            aux = aux->getProximo();
        }
    }
    return 0;
}
template <class E>
E Lista_Generica<E>::removerEle(int i)
{
    Elemento* aux;
    E infoDel;
    if (topo != NULL)
    {
        aux = topo;
        for(int j = 0; j < tamanho(); j++)
        {
            if (j == i)
            {
                break;
            }
            aux = aux->getProximo();
        }
        if (aux->getProximo() != NULL && aux->getAnterior() != NULL)
        {
            aux->getProximo()->setAnterior(aux->getAnterior());
            aux->getAnterior()->setProximo(aux->getProximo());
        }
        else if (aux == topo && aux->getProximo() != NULL)
        {
            topo = topo->getProximo();
            topo->setAnterior(NULL);
        }
        else if(aux == base && aux->getAnterior() != NULL)
        {
            base = base->getAnterior();
            base->setProximo(NULL);
        }
        else
        {
            topo = NULL;
            base = NULL;
        }
        infoDel = aux->getInfo();
        tam --;
        delete aux;
        return infoDel;

    }
    return 0;
}
template <class E>
typename Lista_Generica<E>::Elemento* Lista_Generica<E>::inicio()
{
    return topo;
}
template <class E>
typename Lista_Generica<E>::Elemento* Lista_Generica<E>::fim()
{
    return base;
}
template <class E>
typename Lista_Generica<E>::Elemento* Lista_Generica<E>::retornaElemento(int i)
{
    Elemento* aux;
    if (i <= tam/2)
    {
        if (topo != NULL)
        {
            aux = topo;
            for(int j = 0; j < tam; j++)
            {
                if (j == i)
                    return aux;
                aux = aux->getProximo();
            }
        }
    }
    else if (i > tam/2)
    {
        if (base != NULL)
        {
            aux = base;
            for(int j = tam - 1; j >= 0; j--)
            {
                if (j == i)
                    return aux;
                aux = aux->getAnterior();
            }
        }
    }

    return 0;
}
template <class E>
void Lista_Generica<E>::deletaElemento(Elemento* ele)
{
    if (ele->getAnterior() != NULL)
        ele->getAnterior()->setProximo(ele->getProximo());
    if (ele->getProximo() != NULL)
        ele->getProximo()->setAnterior(ele->getAnterior());
    tam--;
    if (ele == base && ele == topo)
    {
        topo = base = NULL;
    }
    else if (ele == base)
    {
        base = ele->getAnterior();
    }
    else if (ele == topo)
    {
        topo = ele->getProximo();
    }
    ele->setAnterior(NULL);
    ele->setProximo(NULL);
    delete ele;
}


#endif // LISTA_GENERICA_H

