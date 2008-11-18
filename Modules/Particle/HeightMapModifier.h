// Height map modifier.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _DRAGON_PANIC_HEIGHT_MAP_MODIFIER_H_
#define _DRAGON_PANIC_HEIGHT_MAP_MODIFIER_H_

#include "../Island/HeightMap.h"

/**
 * Force a particle to change state upon collision with a height map.
 *
 * @class HeightMapModifier HeightMapModifier.h /DragonPanic/Modules/Particle/HeightMapModifier.h
 */
template <class T> class HeightMapModifier {
private:
    HeightMap& heightMap;
    
public:
    HeightMapModifier(HeightMap& heightMap):
        heightMap(heightMap) {}
    
    virtual ~HeightMapModifier() {}

    inline void Process( T& particle ) {
        Vector<3,float> hPos = heightMap.HeightAt(particle.position);
        if (particle.position[1] < hPos[1]-0.1) {
            particle.position = hPos;
            particle.previousPosition = hPos - Vector<3,float>(0.0,.5,0.0);
            particle.life = 0;
            particle.maxlife = 1500;
            particle.startColor = particle.color;
            particle.endColor = Vector<4,float>(0.2,0.2,0.2,0.3);
        }
    }

};

#endif
