#include <world/life/life-manager.h>
#include <world/life/life-units.h>

using namespace omnetpp;

namespace rfidsim {

Define_Module(LifeManager);

LifeManager::~LifeManager()
{
  for (auto timer: timers)
    cancelAndDelete(timer);
}

void LifeManager::destroy(LifeUnit *lu)
{
  Enter_Method("destroy(%d <%p>)", (lu ? lu->getThingID() : -1), lu);
  if (!lu) return;
  cMessage *timer = new cMessage("LU-%d-Timer", lu->getThingID());
  timer->setControlInfo(lu);
  timers.insert(timer);
  scheduleAt(simTime(), timer);
}

void LifeManager::initialize()
{
  timers.clear();
}

void LifeManager::handleMessage(cMessage *msg)
{
  if (msg->isSelfMessage())
  {
    auto iter = timers.find(msg);
    if (iter != timers.end())
    {
      auto timer = *iter;
      timers.erase(iter);
      auto lu = dynamic_cast<LifeUnit*>(timer->getControlInfo());
      lu->destroy();
    }
  }
}

}
