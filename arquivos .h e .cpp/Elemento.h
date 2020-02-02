#ifndef ELEMENTO_H
#define ELEMENTO_H
#include "CEI.h"

template <class E>
class Elemento
{
public:
    Elemento();
    Elemento(E novo);
    ~Elemento();

    void setProximo(Elemento<E> * p);
    Elemento<E>* getProximo();

    void setAnterior(Elemento<E> * a);
    Elemento<E>* getAnterior();


    void setInfo(E i);
    E getInfo();

private:
    Elemento<E> * proximo;
    Elemento<E> * anterior;
    E info;
};
template <class E>
Elemento<E>::Elemento()
{
    proximo = anterior = NULL;
    info = NULL;
}
template <class E>
Elemento<E>::Elemento(E novo)
{
    proximo = anterior = NULL;
    info = novo;
}
template <class E>
Elemento<E>::~Elemento()
{
}

template <class E>
void Elemento<E>::setProximo(Elemento<E> * p)
{
    proximo = p;
}
template <class E>
Elemento<E>* Elemento<E>::getProximo()
{
    return proximo;
}

template <class E>
void Elemento<E>::setAnterior(Elemento<E> * a)
{
    anterior = a;
}
template <class E>
Elemento<E>* Elemento<E>::getAnterior()
{
    return anterior;
}

template <class E>
void Elemento<E>::setInfo(E i)
{
    info = i;
}
template <class E>
E Elemento<E>::getInfo()
{
    return info;
}

#endif // ELEMENTO_H
