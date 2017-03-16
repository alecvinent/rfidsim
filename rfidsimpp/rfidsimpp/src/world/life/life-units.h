#ifndef RFIDSIMPP_WORLD_LIFE_LIFE_UNITS_H_
#define RFIDSIMPP_WORLD_LIFE_LIFE_UNITS_H_

#include <omnetpp.h>

namespace rfidsim {

class LifeManager;

class LifeUnit : public omnetpp::cSimpleModule {
 public:
  static omnetpp::simsignal_t DESTROY_THING_SIGNAL_ID;

  virtual ~LifeUnit();

  void destroy();

  int getThingID() const { return thing_id; }
  LifeManager *getLifeManager() const { return life_manager; }

 protected:
  virtual void initialize();

 private:
  int thing_id = -1;
  LifeManager *life_manager = nullptr;
};


class LifeTimeUnit : public LifeUnit {
 public:
  virtual ~LifeTimeUnit();

  omnetpp::simtime_t getLifeTime() const { return lifetime; }

 protected:
  virtual void initialize();
  virtual void handleMessage(omnetpp::cMessage *msg);

  virtual void processTimeout();

 private:
  omnetpp::simtime_t lifetime = omnetpp::SimTime::ZERO;
  omnetpp::cMessage *timer = nullptr;
};

}

#endif
