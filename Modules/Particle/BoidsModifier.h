// Boids Modifier
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _DRAGON_PANIC_BOIDS_MODIFIER_H_
#define _DRAGON_PANIC_BOIDS_MODIFIER_H_

#include "../Boid/BoidsSystem.h"


/**
 * Particle Modifier to make boids react to fire
 *
 * @class BoidsModifier BoidsModifier.h /DragonPanic/Modules/Particle/BoidsModifier.h
 */
template <class T> class BoidsModifier {
private:
    BoidsSystem& boidssystem;
public:
    BoidsModifier(BoidsSystem& boidssystem): 
        boidssystem(boidssystem) 
    {
    }

    virtual ~BoidsModifier() {}

    inline void Process( T& particle ) {
        boidssystem.HandleFire(particle.position,30*particle.life/particle.maxlife);
    }

};

#endif
