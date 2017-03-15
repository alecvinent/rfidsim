#ifndef RFIDSIMPP_WORLD_LIFE_LIFE_MANAGER_H_
#define RFIDSIMPP_WORLD_LIFE_LIFE_MANAGER_H_

#include <omnetpp.h>
#include <world/life/life-signals.h>

namespace rfidsim {

class LifeUnit;

class LifeManager : public omnetpp::cSimpleModule {
 public:
  virtual ~LifeManager();
  void destroy(LifeUnit *lu);
 protected:
  virtual void initialize();
  virtual void handleMessage(omnetpp::cMessage *msg);
 private:
  std::set<omnetpp::cMessage *> timers;
};

}

#endif
