
//   utilities.h,  29.1.07, pm-n.

#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include "Vec3.h"

float Abs(float a);
double Abs(double a);
int Abs(int a);

float customFmod( float i, float mMod );
void drawTriangleQuad(Vec3 point[4], Vec3 normal[4], bool flipDiagonal);
void drawColorTriangleQuad(Vec3 point[4], Vec3 normal[4], Vec3 color[4], Vec3 tex[4], bool flipDiagonal);
void drawUVTriangleQuad(Vec3 point[4], Vec3 normal[4], float u[4], float v[4], bool flipDiagonal);

void    draw_pyramid();

double  wall_time();

float   gaussPeak( float my_x, float my_y,
                   float sigma_x, float sigma_y,
                   float x, float y );

#endif
