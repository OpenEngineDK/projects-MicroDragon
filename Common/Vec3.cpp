#include "Vec3.h"
#include <math.h>

Vec3::Vec3(float x, float y, float z) {
 this->x = x;
     this->y = y;
         this->z = z;
             }

Vec3::Vec3(){
 
};

Vec3::~Vec3() {}

Vec3 Vec3::operator +(const Vec3 &vec) {
 Vec3 result(this->x + vec.x,this->y + vec.y,this->z + vec.z);
     return result;
         }

Vec3 Vec3::operator -(const Vec3 &vec) {
 Vec3 result(this->x - vec.x,this->y - vec.y,this->z - vec.z);
     return result;
         }

Vec3 Vec3::operator *(const Vec3 &vec) {
 Vec3 result(this->x * vec.x,this->y * vec.y,this->z * vec.z);
     return result;
         }

Vec3 Vec3::operator /(const Vec3 &vec) {
 Vec3 result(this->x / vec.x,this->y / vec.y,this->z / vec.z);
     return result;
         }

float Vec3::getLength() {
 return sqrtf(this->x*this->x+this->y*this->y+this->z*this->z);
     }

Vec3 Vec3::operator *(const float f) {
 Vec3 result(this->x*f, this->y*f, this->z*f);
     return  result;
         }

Vec3 Vec3::operator /(const float f) {
 Vec3 result(this->x/f, this->y/f, this->z/f);
     return  result;
         }

Vec3 Vec3::normalize() {
 return (*this)*(1/this->getLength());
     }

Vec3 Vec3::truncate(float maxLength) {
 if (this->getLength()<=maxLength) return (*this);
                                       else return this->normalize()*maxLength;
                                       }

float Vec3::getDot(const Vec3 &vec) {
 return (this->x * vec.x + this->y * vec.y + this->z * vec.z);
     }

float Vec3::getAngle(const Vec3 &vec) {
 Vec3 temp = vec;
     return acos( this->getDot(vec) / (this->getLength()*(temp.getLength())) ) * (180/3.14159265358979323846f);
         };

Vec3 Vec3::getCross(const Vec3 &vec){
 Vec3 result (this->y * vec.z - vec.y * this->z, this->z * vec.x - vec.z * this->x, this->x * vec.y - vec.x * this->y);
     return result;
         }

Vec3 Vec3::getNormal(const Vec3 &vec) {
 Vec3 result(0,0,0);
     Vec3 cross = this->getCross(vec);
         if(!cross.getLength()==0) { result = cross.normalize(); }
                                       return result;
                                           }

Vec3 Vec3::rotateAround(const Vec3 &axis, float angle) {
 float co = cos(angle);
     float si = sin(angle);
         Vec3 temp = axis;
             Vec3 a = temp.normalize();
                 float ax = a.x;
                     float ay = a.y;
                         float az = a.z;
                             float i11 = co+ax*ax*(1-co);
                                 float i12 = ax*ay*(1-co)-az*si;
                                     float i13 = ay*si+ax*az*(1-co);
                                         float i21 = az*si+ax*ay*(1-co);
                                             float i22 = co+ay*ay*(1-co);
                                                 float i23 = -ax*si+ay*az*(1-co);
                                                     float i31 = -ay*si+ax*az*(1-co);
                                                         float i32 = ax*si+ay*az*(1-co);
                                                             float i33 = co+az*az*(1-co);
                                                                 return Vec3(
                                                                             i11*x+i12*y+i13*z,
                                                                             i21*x+i22*y+i23*z,
                                                                             i31*x+i32*y+i33*z
                                                                             );
                                                                     }

void Vec3::setValues(float x, float y, float z) {
 this->x = x;
     this->y = y;
         this->z = z;
             }

ostream& operator<< (ostream& out, const Vec3& vec)
{
 out << "<" << vec.x << ", " << vec.y << ", " << vec.z << ">";
     return out;
         }

//Vec3 operator <<() {
//	cout "<" << x << ", " << y << ", " << z << ">";
//}
