#ifndef RFIDSIMPP_WORLD_LIFE_LIFE_UNITS_H_
#define RFIDSIMPP_WORLD_LIFE_LIFE_UNITS_H_

#include <omnetpp.h>

namespace rfidsim {

class LifeUnit : public omnetpp::cSimpleModule {
 public:
  static omnetpp::simsignal_t DESTROY_THING_SIGNAL_ID;

  virtual ~LifeUnit();

  void destroy();

  int getThingID() const { return thing_id; }

 protected:
  virtual void initialize();

 private:
  int thing_id;
};


class LifeTimeUnit : public LifeUnit {
 public:
  virtual ~LifeTimeUnit();

  omnetpp::simtime_t getLifeTime() const { return lifetime; }

 protected:
  virtual void initialize();
  virtual void handleMessage(omnetpp::cMessage *msg);

 private:
  omnetpp::simtime_t lifetime = omnetpp::SimTime::ZERO;
  omnetpp::cMessage *timer = nullptr;
};

}

#endif
