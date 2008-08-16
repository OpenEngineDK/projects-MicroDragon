#ifndef _BOID_SYSTEM_EVENT_
#define _BOID_SYSTEM_EVENT_

enum BoidEvent { BOID_DIED };

struct BoidSystemEventArg {
    BoidEvent boidEvent;
    Vector<3,float> position;
BoidSystemEventArg(BoidEvent boidEvent, Vector<3,float> position)
 : boidEvent(boidEvent), position(position) { } 
};

#endif // _BOID_SYSTEM_EVENT_
