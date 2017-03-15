#include <world/life/life-units.h>
#include <world/life/life-signals.h>

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
  //TODO
}


//
//===========================================================================
// LifeTimeUnit
//===========================================================================
//
LifeTimeUnit::~LifeTimeUnit()
{
  //TODO
}

void LifeTimeUnit::initialize()
{
  //TODO
}

void LifeTimeUnit::handleMessage(cMessage *msg)
{
  //TODO
}

}
