#include "Vetor_R2.h"


Vetor_R2::Vetor_R2()
{
    x = 0;
    y = 0;
}
Vetor_R2::Vetor_R2(const float px, const float py)
{
    x = px;
    y = py;
}

Vetor_R2::~Vetor_R2()
{
}

Vetor_R2& Vetor_R2::operator=(Vetor_R2 vec)
{
    x = vec.getX();
    y = vec.getY();
    return *this;
}
Vetor_R2& Vetor_R2::operator+= (Vetor_R2 vec)
{
    x += vec.getX();
    y += vec.getY();
    return *this;
}
Vetor_R2 Vetor_R2::operator+ (Vetor_R2 vec1)
{
    vec1 += *this;
    return vec1;
}
Vetor_R2& Vetor_R2::operator-= (Vetor_R2& vec)
{
    x -= vec.getX();
    y -= vec.getY();
    return *this;
}
Vetor_R2 Vetor_R2::operator- (Vetor_R2 vec1)
{
    vec1 -= *this;
    return vec1;
}
Vetor_R2& Vetor_R2::operator*= (float i)
{
    x *= i;
    y *= i;
    return *this;
}
Vetor_R2 Vetor_R2::operator* (float i)
{
    Vetor_R2 vec;
    vec.setX(this->getX()*i);
    vec.setY(this->getY()*i);
    return vec;
}
void Vetor_R2::setX(const float px)
{
    x = px;
}
float Vetor_R2::getX() const
{
    return x;
}
void Vetor_R2::setY(const float py)
{
    y = py;
}
float Vetor_R2::getY() const
{
    return y;
}
void Vetor_R2::setXY(const float px, const float py)
{
    x = px;
    y = py;
}
Vetor_R2 Vetor_R2::vetorNulo()
{
    Vetor_R2 vetor(0, 0);
    return vetor;
}
Vetor_R2 Vetor_R2::vetor(float px, float py)
{
    Vetor_R2 vetor(px, py);
    return vetor;
}
Vetor_R2 Vetor_R2::copiaVetor(Vetor_R2 vec)
{
    return vec;
}
