#include "GameState.h"

#include "Modules/Boid/BoidsSystem.h"
#include "Modules/Boid/BoidSystemEvent.h"

#include <Logging/Logger.h>

GameState::GameState(unsigned int duration) : duration(duration) {
  timer.Start();
  numberOfDeadBoids = 0;
}

GameState::~GameState() {}

void GameState::Handle(ProcessEventArg arg) {

}

void GameState::Handle(BoidSystemEventArg arg) {
  if (arg.boidEvent == BOID_DIED) numberOfDeadBoids++;
}

/** returns time in seconds */
unsigned int GameState::GetTime() {
  return timer.GetElapsedIntervals(1000000);
}

/** returns time left in seconds */
unsigned int GameState::GetTimeLeft() {
  if(duration<=GetTime()) return 0;
  return duration -GetTime();
}

unsigned int GameState::GetScore() {
  return numberOfDeadBoids;
}
