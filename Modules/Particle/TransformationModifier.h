// transformation node modifier.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _DRAGON_PANIC_TRANSFORMATION_MODIFIER_H_
#define _DRAGON_PANIC_TRANSFORMATION_MODIFIER_H_

#include <Scene/TransformationNode.h>
#include <Math/Vector.h>
#include <Math/Quaternion.h>

using OpenEngine::Scene::TransformationNode;
using OpenEngine::Math::Vector;
using OpenEngine::Math::Quaternion;

/**
 * Update a particle system's TransformationNode and hereby the spawn position.
 *
 * @class TransformationModifier TransformationModifier.h /DragonPanic/Modules/Particle/TransformationModifier.h
 */
template <class T> class TransformationModifier {
private:
    TransformationNode* node;
    float speed;
public:
    TransformationModifier(TransformationNode* node, float speed):
        node(node), speed(speed) {}
    
    virtual ~TransformationModifier() {}
    
    inline void Process( T& particle ) {
        Quaternion<float> q;
        Vector<3,float> deltapos;
        
        node->GetAccumulatedTransformations(&deltapos, &q);
        q.Normalize();

        deltapos = q.RotateVector(Vector<3,float>(0.0,-1.0,0.0)) * 0.035;// * speed;
        node->SetPosition(node->GetPosition()-deltapos);
        //logger.info << "handle transmod: " << node->GetPosition() << logger.end;
    }
};

#endif
