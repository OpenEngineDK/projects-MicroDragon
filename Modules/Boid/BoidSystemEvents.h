#ifndef _BOID_SYSTEM_EVENTS_
#define _BOID_SYSTEM_EVENTS_

#include <Math/Vector.h>

using OpenEngine::Math::Vector;

enum BoidEvent { BOID_DIED };

struct BoidSystemEventArg {
    BoidEvent boidEvent;
    Vector<3,float> position;
BoidSystemEventArg(BoidEvent boidEvent, Vector<3,float> position)
 : boidEvent(boidEvent), position(position) { } 
};

#endif // _BOID_SYSTEM_EVENT_
