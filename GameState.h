#ifndef _GAME_STATE_
#define _GAME_STATE_

#include <Core/IListener.h>
#include <Core/EngineEvents.h>
#include <Utils/Timer.h>

using OpenEngine::Core::IListener;
using OpenEngine::Core::ProcessEventArg;
using OpenEngine::Utils::Timer;

class BoidsSystem;
struct BoidSystemEventArg;

class GameState : public IListener<ProcessEventArg>,
  public IListener<BoidSystemEventArg> {
 private:
  unsigned int numberOfDeadBoids, duration;
  Timer timer;
 public:
  GameState(unsigned int duration);
  virtual ~GameState();

  void Handle(ProcessEventArg);
  void Handle(BoidSystemEventArg arg);

  unsigned int GetTime();
  unsigned int GetTimeLeft();
  unsigned int GetScore();

  void Reset();

};

#endif // _GAME_STATE_
