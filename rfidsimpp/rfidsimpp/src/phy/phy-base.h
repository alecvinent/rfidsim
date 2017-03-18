#ifndef RFIDSIMPP_PHY_PHY_BASE_H_
#define RFIDSIMPP_PHY_PHY_BASE_H_

#include <omnetpp.h>

namespace rfidsim {

class Phy : public omnetpp::cSimpleModule, public omnetpp::cListener {
 public:
  virtual ~Phy();

 protected:
  virtual void initialize();

  virtual void handleMessage(omnetpp::cMessage *msg);

  virtual void receiveSignal(
          omnetpp::cComponent *source, omnetpp::simsignal_t signal_id,
          omnetpp::cObject *obj, omnetpp::cObject *details);
};

}

#endif
