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

#include "../Island/HeightMap.h"
#include "Explosion.h"

#include <Scene/TransformationNode.h>
#include <Math/Vector.h>
#include <Math/Quaternion.h>
#include <Effects/FireEffect.h>

using OpenEngine::Scene::TransformationNode;
using OpenEngine::Math::Vector;
using OpenEngine::Math::Quaternion;
using OpenEngine::Effects::FireEffect;

/**
 * Update a particle system's TransformationNode and hereby the spawn position.
 *
 * @class TransformationModifier TransformationModifier.h /DragonPanic/Modules/Particle/TransformationModifier.h
 */
template <class T> class TransformationModifier {
private:
    FireEffect& fire;
    TransformationNode* node;
    TransformationNode* tmpnode;
    float speed;
    bool active;
    HeightMap& heightMap;
    Vector<3,float> deltapos, gravity;
    Explosion& exp;
public:
    TransformationModifier(FireEffect& fire, float speed, HeightMap& heightMap,
                           Explosion& exp):
        fire(fire),
        node(fire.GetTransformationNode()), 
        tmpnode(new TransformationNode()),
        speed(speed), active(false), 
        heightMap(heightMap),
        gravity(Vector<3,float>(0,-0.07,0)),
        exp(exp) {
    }
    
    virtual ~TransformationModifier() {
        delete tmpnode;
    }
    
    void SetSpeed(float speed) { this->speed = speed; }

    float GetSpeed() { return speed; }

    void SetActive(bool active) {
        this->active = active;
        if (active) {
            node = fire.GetTransformationNode();
            Quaternion<float> q;
            Vector<3,float> pos;
            node->GetAccumulatedTransformations(&pos, &q);
            deltapos = q.RotateVector(Vector<3,float>(0.0,-1.0,0.0)) * speed;
            tmpnode->SetPosition(pos);
            tmpnode->SetRotation(q);
            fire.SetTransformationNode(tmpnode);
        }
        else {
            fire.SetTransformationNode(node);
        }
    }

    inline void Process() {
        if (!active) return;
        
        tmpnode->SetPosition(tmpnode->GetPosition()+deltapos);
        deltapos *= 0.99;
        deltapos += gravity;

        Vector<3,float> h = heightMap.HeightAt(tmpnode->GetPosition());
        if (tmpnode->GetPosition()[1] < h[1]-0.1) {
            SetActive(false);
            fire.SetActive(false);
            fire.Reset();
            exp.Fire(tmpnode->GetPosition());
        }
    }
};

#endif
