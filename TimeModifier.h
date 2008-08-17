#ifndef _TIME_MODIFIER_
#define _TIME_MODIFIER_

#include <Core/Event.h>
#include <Core/EngineEvents.h>
#include <Core/IListener.h>
#include <Core/IEvent.h>

using OpenEngine::Core::IListener;
using OpenEngine::Core::ProcessEventArg;
using OpenEngine::Core::IEvent;
using OpenEngine::Core::Event;

class TimeModifier : public IListener<ProcessEventArg> {
 private:
    IEvent<ProcessEventArg>& originalEvents;
    Event<ProcessEventArg>  newEvents;
    float factor;

public:
   TimeModifier(IEvent<ProcessEventArg>& originalEvents, float factor) 
     : originalEvents(originalEvents), factor(factor) {
      originalEvents.Attach(*this);
    }

    virtual ~TimeModifier() {}

  void Handle(ProcessEventArg arg) {
    ProcessEventArg event(arg.start, arg.approx*factor);
    newEvents.Notify(event);
  }

  void SetFactor(float factor) {
    this->factor = factor;
  }

  virtual IEvent<ProcessEventArg>& ProcessEvent() {
    return newEvents;
  }

};

#endif // _TIME_MODIFIER_
