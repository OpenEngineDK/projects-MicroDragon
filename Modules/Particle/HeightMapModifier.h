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
#include <Math/Quaternion.h>

using OpenEngine::Math::Quaternion;

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
                    
            Quaternion<float> q(PI, heightMap.NormalAt(particle.position));
            q.Normalize();
            
            particle.velocity = q.RotateVector(particle.velocity);
            particle.velocity = Vector<3,float>(0.0,1.0,0.0)*particle.velocity.GetLength();
            particle.velocity *= 0.5;
        }
    }

};

#endif
