#ifndef _COORDS_H_
#define _COORDS_H_

#define SQR(x) ((x)*(x))
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

class FVector
{
public:
    union {float X;float R;};
    union {float Y;float G;};
    union {float Z;float B;};

    FVector() {};
    FVector(float x, float y, float z) {X=x;Y=y;Z=z;};
    
    float Magnitude() const {return (float)sqrt(SQR(X)+SQR(Y)+SQR(Z));};
    float MagnitudeSq() const {return (float)(SQR(X)+SQR(Y)+SQR(Z));};
    void Normalise() {float l=1.f/Magnitude();X*=l;Y*=l;Z*=l;};
    float operator*(const FVector &a) const  {return X*a.X+Y*a.Y+Z*a.Z;};
    FVector operator*(const float a) const {return FVector(X*a,Y*a,Z*a);};
    FVector operator+(const FVector &a) const  {return FVector(X+a.X,Y+a.Y,Z+a.Z);};
    FVector operator-(const FVector &a) const {return FVector(X-a.X,Y-a.Y,Z-a.Z);};
    FVector operator^(const FVector &a) const {return FVector(Y*a.Z-Z*a.Y,Z*a.X-X*a.Z,X*a.Y-Y*a.X);};
    void operator*=(const float a) {X*=a;Y*=a;Z*=a;};
    void operator+=(const FVector &a) {X+=a.X;Y+=a.Y;Z+=a.Z;};
    void operator-=(const FVector &a) {X-=a.X;Y-=a.Y;Z-=a.Z;};

};

class FMatrix
{
public:
    FVector Row[3];
    FMatrix(FVector a,FVector b, FVector c) {Row[0]=a;Row[1]=b;Row[2]=c;};
    FMatrix() {};

    FVector Column0() const {return FVector(Row[0].X,Row[1].X,Row[2].X);};
    FVector Column1() const {return FVector(Row[0].Y,Row[1].Y,Row[2].Y);};
    FVector Column2() const {return FVector(Row[0].Z,Row[1].Z,Row[2].Z);};
#define MAKEROT(II,JJ,KK,II2,JJ2,KK2) \
        float c=(float)cos(theta);    \
        float s=(float)sin(theta);    \
        Row[II].II2=c;Row[II].JJ2=s;Row[II].KK2=0;\
        Row[JJ].II2=-s;Row[JJ].JJ2=c;Row[JJ].KK2=0;\
        Row[KK].II2=0;Row[KK].JJ2=0;Row[KK].KK2=1;
    void MakeXRot(float theta) {MAKEROT(1,2,0,Y,Z,X);};
    void MakeYRot(float theta) {MAKEROT(2,0,1,Z,X,Y);};
    void MakeZRot(float theta) {MAKEROT(0,1,2,X,Y,Z);};
    void MakeID() {Row[0]=FVector(1,0,0);Row[2]=FVector(0,0,1);Row[1]=FVector(0,1,0);};
    FMatrix Transpose() const {return FMatrix(Column0(),Column1(),Column2());};
    void TransposeInPlace() {FMatrix m=Transpose();*this=m;};
    void Normalise()
    {
        Row[2].Normalise();
        Row[0]=Row[1]^Row[2];
        Row[0].Normalise();
        Row[1]=Row[2]^Row[0];
        Row[1].Normalise();
    };
    FMatrix operator*(const float a) const {return FMatrix(Row[0]*a,Row[1]*a,Row[2]*a);};
    FMatrix operator*(const FMatrix &a) const {return FMatrix(
        FVector(Row[0].X*a.Row[0].X+Row[0].Y*a.Row[1].X+Row[0].Z*a.Row[2].X,
                Row[0].X*a.Row[0].Y+Row[0].Y*a.Row[1].Y+Row[0].Z*a.Row[2].Y,
                Row[0].X*a.Row[0].Z+Row[0].Y*a.Row[1].Z+Row[0].Z*a.Row[2].Z),

        FVector(Row[1].X*a.Row[0].X+Row[1].Y*a.Row[1].X+Row[1].Z*a.Row[2].X,
                Row[1].X*a.Row[0].Y+Row[1].Y*a.Row[1].Y+Row[1].Z*a.Row[2].Y,
                Row[1].X*a.Row[0].Z+Row[1].Y*a.Row[1].Z+Row[1].Z*a.Row[2].Z),

        FVector(Row[2].X*a.Row[0].X+Row[2].Y*a.Row[1].X+Row[2].Z*a.Row[2].X,
                Row[2].X*a.Row[0].Y+Row[2].Y*a.Row[1].Y+Row[2].Z*a.Row[2].Y,
                Row[2].X*a.Row[0].Z+Row[2].Y*a.Row[1].Z+Row[2].Z*a.Row[2].Z));};

    FVector operator*(const FVector &a) const {return FVector(a*Row[0],a*Row[1],a*Row[2]);};
    FMatrix operator+(const FMatrix &a) const {return FMatrix(Row[0]+a.Row[0],Row[1]+a.Row[1],Row[2]+a.Row[2]);};
    FMatrix operator-(const FMatrix &a) const {return FMatrix(Row[0]+a.Row[0],Row[1]+a.Row[1],Row[2]+a.Row[2]);};

};


#endif