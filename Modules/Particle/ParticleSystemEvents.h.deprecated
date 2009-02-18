#ifndef _PARTICLE_SYSTEM_EVENTS_
#define _PARTICLE_SYSTEM_EVENTS_

#include <Math/Vector.h>

using OpenEngine::Math::Vector;

enum ParticleEvent { EXPLOTION };

struct ParticleSystemEventArg {
    ParticleEvent boidEvent;
    Vector<3,float> position;
    float strength;

ParticleSystemEventArg(ParticleEvent boidEvent, Vector<3,float> position, float strength)
: boidEvent(boidEvent), position(position), strength(strength) { } 
};

#endif // _PARTICLE_SYSTEM_EVENTS_
