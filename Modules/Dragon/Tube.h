#ifndef _TUBE_H_
#define _TUBE_H_

#include "../../Common/Vec3.h"
//#include "../../Common/gltga.h"

class Tube {
public:
    Tube(float radius1, float radius2, float stiffness1, float stiffness2, float length);
    ~Tube();
    void update(Vec3 pointA, Vec3 dirA, Vec3 upA, Vec3 pointB, Vec3 dirB, Vec3 upB, bool updateTwisting);
    void draw();
    const static int samples = 50;
    const static int links = 50;

    float getLinkRadius(int linkIndex);
    Vec3 getLinkX(int linkIndex);
    Vec3 getLinkY(int linkIndex);
    Vec3 getLinkZ(int linkIndex);
    Vec3 getLinkPosition(int linkIndex);
    float* getLinkMatrix(int linkIndex);

private:
    Vec3 p0, p1, p2, p3;
    Vec3 z1, z2;
    Vec3 y1, y2;
    float radius1, radius2;
    float length;

    float adjuster;
    float twistAngle;
    float stiffness1, stiffness2;
    float sample[samples];
    float linkR[links];
    float linkT[links];
    Vec3 linkX[links];
    Vec3 linkY[links];
    Vec3 linkZ[links];
    Vec3 linkP[links];
    float linkM[links][16];

    Vec3 spline(float s);

    unsigned int dragonNeckTextureID;
};

#endif
