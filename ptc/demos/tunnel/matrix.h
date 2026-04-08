////////////////////////////////////////////
// Minimalist 4x4 matrix class            //
// Copyright (c) 1998 by BLACKAXE / KoLOr //
////////////////////////////////////////////

#ifndef __MATRIX_H
#define __MATRIX_H

#include <math.h>
#include <stdlib.h>





// pi
#ifndef PI
#define PI 3.14159265359
#endif





////////////////////////////////////////////////////////////////////////////////////////////////////
// matrix handling class, handles a 4x4 transformation Matrix usually used in Computer Graphics
// (3x3 rotation part) and 1x3 translation part
class Matrix {
    friend class Vector;
    
    protected:
        float data[4][4];

    public:
        inline Matrix();                       // default constructors, initializes an identify matrix
        inline Matrix(const float, const float, const float, const float,
                      const float, const float, const float, const float,
                      const float, const float, const float, const float,
                      const float, const float, const float, const float);

        inline void Set(const int i, const int j, const float f) {
            data[i][j] = f;
        }

        inline const Matrix& operator = (const Matrix&);
        inline Matrix(const Matrix&);

        // Matrix multiplication
        inline Matrix operator * (const Matrix&) const;
};


inline Matrix::Matrix()
{
    for(int i=0; i < 4; i++)
        for(int j=0; j < 4; j++) {
            data[i][j] = 0.0;
            data[i][i] = 1.0;                   // make an identify matrix
        }    
}

inline Matrix::Matrix(const float f00, const float f01, const float f02, const float f03,
                      const float f10, const float f11, const float f12, const float f13,
                      const float f20, const float f21, const float f22, const float f23,
                      const float f30, const float f31, const float f32, const float f33)
{
    data[0][0] = f00; data[0][1] = f01; data[0][2] = f02; data[0][3] = f03;
    data[1][0] = f10; data[1][1] = f11; data[1][2] = f12; data[1][3] = f13;
    data[2][0] = f20; data[2][1] = f21; data[2][2] = f22; data[2][3] = f23;
    data[3][0] = f30; data[3][1] = f31; data[3][2] = f32; data[3][3] = f33;
}


// copy constructor
inline Matrix::Matrix(const Matrix &m)
{
    for(int i=0; i < 4; i++)
        for(int j=0; j < 4; j++)
            data[i][j] = m.data[i][j];
}


inline const Matrix& Matrix::operator = (const Matrix &m)
{
    if (this == &m) return *this;

    for(int i=0; i < 4; i++)
        for(int j=0; j < 4; j++)
            data[i][j] = m.data[i][j];

    return *this;
}


inline Matrix Matrix::operator * (const Matrix &m) const
{
    Matrix result;
    int i,j,k;
    float sum;

    for(i=0; i < 4; i++) {
        for(j=0; j < 4; j++) {
            sum=0;
            for(k=0; k < 4; k++)
                sum += data[i][k] * m.data[k][j];
            result.data[i][j]=sum;
        }
    }
    return result;
}






#endif