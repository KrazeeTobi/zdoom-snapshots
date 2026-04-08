////////////////////////////////////////////
// Minimal 3d vector class                //
// Copyright (c) 1998 by BLACKAXE / KoLOr //
////////////////////////////////////////////

#ifndef __VECTOR_H
#define __VECTOR_H

#include <math.h>
#include <stdlib.h>






/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Vector Handling class,  contains only things needed for this special effect, otherwise this source file would have
// had considerably grown, and Gaffer wouldn't have been very happy ;-)
// i leave it to you to complete this class and the Matrix class, with everything you need
class Vector {
    friend class Matrix;    
    
    protected:
        float x,y,z;

    public:
        inline Vector() { x = y = z = 0.0; }
        inline Vector(const float, const float, const float);

        inline float GetX() const { return x; }
        inline float GetY() const { return y; }
        inline float GetZ() const { return z; }

        // scalar product
        inline friend Vector operator * (const float f, const Vector &v) {
            return Vector(f*v.x, f*v.y, f*v.z);
        }

        // product of a vector with a Matrix
        inline const Vector& operator *= (const Matrix&);

        // addition of 2 vectors
        inline Vector operator + (const Vector &v) const {
            return Vector(x+v.x, y+v.y, z+v.z);
        }

        // substraction of 2 vectors
        inline Vector operator - (const Vector &v) const {
            return Vector(x-v.x, y-v.y, z-v.z);
        }    

        inline float Length() const {
            return sqrt(x*x + y*y + z*z);
        }
        inline void Normalize();
};


inline Vector::Vector(const float tx, const float ty, const float tz)
{
    x = tx;
    y = ty;
    z = tz;
}


inline const Vector& Vector::operator *= (const Matrix &m)
{
    float tx = x*m.data[0][0] + y*m.data[1][0] + z*m.data[2][0] + m.data[3][0];
    float ty = x*m.data[0][1] + y*m.data[1][1] + z*m.data[2][1] + m.data[3][1];
    float tz = x*m.data[0][2] + y*m.data[1][2] + z*m.data[2][2] + m.data[3][2];

    x = tx;
    y = ty;
    z = tz;

    return *this;
}


inline void Vector::Normalize() 
{
    float recip = 1.0/Length();
 
    x *= recip;
    y *= recip;
    z *= recip;
}





#endif