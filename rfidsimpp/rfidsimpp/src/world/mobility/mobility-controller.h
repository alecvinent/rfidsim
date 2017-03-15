#ifndef RFIDSIMPP_WORLD_MOBILITY_MOBILITY_CONTROLLER_H_
#define RFIDSIMPP_WORLD_MOBILITY_MOBILITY_CONTROLLER_H_

#include <omnetpp.h>

namespace rfidsim {

class MobilityController : public omnetpp::cSimpleModule {
 public:
  static omnetpp::simsignal_t UPDATE_POSITION_SIGNAL_ID;

  virtual ~MobilityController();

 protected:
  virtual void initialize();
  virtual void handleMessage(omnetpp::cMessage *msg);

  virtual void processTimer(omnetpp::cMessage *timer);

 private:
  omnetpp::simtime_t interval;
  omnetpp::cMessage *timer = nullptr;
};

}


#endif /* MOBILITY_MOBILITY_CONTROLLER_H_ */
