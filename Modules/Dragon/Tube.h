#ifndef _TUBE_H_
#define _TUBE_H_

// using templates
#include <Math/Vector.h>

using OpenEngine::Math::Vector;

class Tube {
public:
    Tube(float radius1, float radius2, 
	 float stiffness1, float stiffness2, float length);
    ~Tube();
    void update(Vector<3,float> pointA, Vector<3,float> dirA, 
		Vector<3,float> upA, Vector<3,float> pointB,
		Vector<3,float> dirB, Vector<3,float> upB,
		bool updateTwisting);
    void draw();
    const static int samples = 50;
    const static int links = 50;

    float getLinkRadius(int linkIndex);
    Vector<3,float> getLinkX(int linkIndex);
    Vector<3,float> getLinkY(int linkIndex);
    Vector<3,float> getLinkZ(int linkIndex);
    Vector<3,float> getLinkPosition(int linkIndex);
    float* getLinkMatrix(int linkIndex);

private:
    Vector<3,float> p0, p1, p2, p3;
    Vector<3,float> z1, z2;
    Vector<3,float> y1, y2;
    float radius1, radius2;
    float length;

    float adjuster;
    float twistAngle;
    float stiffness1, stiffness2;
    float sample[samples];
    float linkR[links];
    float linkT[links];
    Vector<3,float> linkX[links];
    Vector<3,float> linkY[links];
    Vector<3,float> linkZ[links];
    Vector<3,float> linkP[links];
    float linkM[links][16];

    Vector<3,float> spline(float s);

    unsigned int dragonNeckTextureID;
};

#endif
