#ifndef VETOR_R2_H
#define VETOR_R2_H



class Vetor_R2
{
    public:
        Vetor_R2();
        Vetor_R2(const float px, const float py);
        ~Vetor_R2();

        Vetor_R2& operator=(Vetor_R2 vec);
        Vetor_R2& operator+= (Vetor_R2 vec);
        Vetor_R2 operator+ (Vetor_R2 vec1);
        Vetor_R2& operator-= (Vetor_R2& vec);
        Vetor_R2 operator- (Vetor_R2 vec1);
        Vetor_R2& operator*= (float i);
        Vetor_R2 operator * (float i);

        void setX(const float px);
        float getX() const;
        void setY(const float py);
        float getY() const;
        void setXY(const float px, const float py);

        static Vetor_R2 vetorNulo();
        static Vetor_R2 vetor(float px, float py);
        static Vetor_R2 copiaVetor(Vetor_R2 vec);
    private:
        float x;
        float y;


};

#endif // VETOR_R2_H
