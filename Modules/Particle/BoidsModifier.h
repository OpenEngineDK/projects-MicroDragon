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
    float strength;
public:
 BoidsModifier(BoidsSystem& boidssystem, float strength): 
    boidssystem(boidssystem), strength(strength) {}
    virtual ~BoidsModifier() {}

    inline void Process(float dt, T& particle) {
        boidssystem.HandleFire(particle.position, 
                               dt*strength*particle.life/particle.maxlife);
    }

    void SetStrength(float strength) {
        this->strength = strength;
    }

    float GetStrength() { 
        return strength; 
    }
};

#endif
