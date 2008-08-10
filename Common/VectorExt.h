#ifndef _VECTOR_EXT_
#define _VECTOR_EXT_

#include <Math/Math.h>

using OpenEngine::Math::PI;

class VectorExt {
 public:
  static Vector<3,float> RotateAround(Vector<3,float> ths, Vector<3,float> axis, float angle) {
    float co = cos(angle);
    float si = sin(angle);
    Vector<3,float> temp = axis;
    Vector<3,float> a = temp;
    if(!temp.IsZero())
      temp.Normalize();
    float ax = a[0];
    float ay = a[1];
    float az = a[2];
    float i11 = co+ax*ax*(1-co);
    float i12 = ax*ay*(1-co)-az*si;
    float i13 = ay*si+ax*az*(1-co);
    float i21 = az*si+ax*ay*(1-co);
    float i22 = co+ay*ay*(1-co);
    float i23 = -ax*si+ay*az*(1-co);
    float i31 = -ay*si+ax*az*(1-co);
    float i32 = ax*si+ay*az*(1-co);
    float i33 = co+az*az*(1-co);
    return Vector<3,float>(
			   i11*ths[0]+i12*ths[1]+i13*ths[2],
			   i21*ths[0]+i22*ths[1]+i23*ths[2],
			   i31*ths[0]+i32*ths[1]+i33*ths[2]
			   );
  }
  
  static float GetAngle(Vector<3,float> left, Vector<3,float> right) {
    Vector<3,float> temp = right;
    return acos( (left*right) / (left.GetLength()*(temp.GetLength())) )*(180/PI);
  }

  static Vector<3,float> Truncate(Vector<3,float> ths, float maxLength) {

    if (ths.GetLength()<=maxLength) return (ths);
    else return ths.GetNormalize()*maxLength;
 }
};

#endif
