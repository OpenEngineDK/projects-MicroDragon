#ifndef __VEC3_H__
#define __VEC3_H__

#include <fstream>

using namespace std;

class Vec3 {
public:
    Vec3(float x, float y, float z);
    Vec3();
    ~Vec3();
    Vec3 operator+(const Vec3&);
    Vec3 operator-(const Vec3&);
    Vec3 operator*(const Vec3&);
    Vec3 operator/(const Vec3&);
    friend ostream& operator << (ostream& out, const Vec3& vec);
    float getLength();
    Vec3 operator *(const float f);
    Vec3 operator /(const float f);
    Vec3 normalize();
    Vec3 truncate(float maxLength);
    Vec3 getCross(const Vec3&);
    Vec3 getNormal(const Vec3&);
    Vec3 rotateAround(const Vec3&, float angle);
    float getAngle(const Vec3&);
    float getDot(const Vec3&);
    void setValues(float, float, float);
  
    float x, y, z;

};

#endif
