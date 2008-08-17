// Play sounds by event triggering
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Sound/ISoundSystem.h>
#include <Sound/IMonoSound.h>
#include <Resources/ISoundResource.h>
#include <Math/Vector.h>

using OpenEngine::Sound::ISoundSystem;
using OpenEngine::Sound::ISoundResourcePtr;
using OpenEngine::Sound::IMonoSound;
using OpenEngine::Math::Vector;

/**
 * Short description.
 *
 * @class GameSound GameSound.h ts/DragonPanic/GameSound.h
 */
class Voice {
private:
    ISoundSystem& soundsystem;
    IMonoSound* screamsound;
public:
    Voice(ISoundSystem& soundsystem);
    ~Voice();

    void SetPosition(Vector<3,float> pos);
    void Scream();
};
