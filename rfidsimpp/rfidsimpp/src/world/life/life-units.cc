#include <world/life/life-units.h>
#include <world/life/life-signals.h>
#include <world/life/life-manager.h>
#include <common/module-access.h>

using namespace omnetpp;

namespace rfidsim {

Define_Module(LifeUnit);
Define_Module(LifeTimeUnit);

//
//===========================================================================
// LifeUnit
//===========================================================================
//

simsignal_t LifeUnit::DESTROY_THING_SIGNAL_ID =
        registerSignal(DestroyThing::NAME);


LifeUnit::~LifeUnit()
{
  // nothing to be done here
}

void LifeUnit::destroy()
{
  Enter_Method("destroy()");
  DestroyThing signal_(thing_id);
  emit(DESTROY_THING_SIGNAL_ID, &signal_);
}


void LifeUnit::initialize()
{
  thing_id = static_cast<int>(par("thingID").longValue());
  life_manager = findChildByType<LifeManager>(getSystemModule());
}


//
//===========================================================================
// LifeTimeUnit
//===========================================================================
//
LifeTimeUnit::~LifeTimeUnit()
{
  cancelAndDelete(timer);
}

void LifeTimeUnit::initialize()
{
  LifeUnit::initialize();
  lifetime = par("lifetime").doubleValue();

  char timer_name[64];
  snprintf(timer_name, sizeof(timer_name), "LTU-%d-Timer", getThingID());
  timer = new cMessage(timer_name);

  scheduleAt(simTime() + lifetime, timer);
}

void LifeTimeUnit::handleMessage(cMessage *msg)
{
  if (msg == timer)
    processTimeout();
  else
    throw cRuntimeError("unexpected message '%s'", msg->getFullName());
}

void LifeTimeUnit::processTimeout()
{
  getLifeManager()->destroy(this);
}

}
