#ifndef RFIDSIMPP_RADIO_TRANSCEIVERS_H_
#define RFIDSIMPP_RADIO_TRANSCEIVERS_H_

#include <omnetpp.h>
#include <common/geometry.h>
#include <common/units.h>

namespace rfidsim {


class TransceiverBase : public omnetpp::cSimpleModule {
 public:

  virtual ~TransceiverBase() {}

  virtual void fieldUpdated(TransceiverBase *peer) = 0;

 protected:

  virtual void updateField() = 0;

  virtual int numInitStages() const {
    //TODO
    return 1;
  }

  virtual void initialize(int stage) {
    //TODO
  }

  virtual void handleMessage(omnetpp::cMessage *msg) {
    //TODO
  }

};


class ActiveTransceiver : public TransceiverBase {
 public:

  virtual ~ActiveTransceiver() {}

 protected:

  virtual void initialize(int stage) {
    TransceiverBase::initialize(stage);
    //TODO
  }

  virtual void handleMessage(omnetpp::cMessage *msg) {
    //TODO
  }

};


class PassiveTransceiver : public TransceiverBase {
 public:

  virtual ~PassiveTransceiver() {}

 protected:

  virtual void initialize(int stage) {
    TransceiverBase::initialize(stage);
    //TODO
  }

  virtual void handleMessage(omnetpp::cMessage *msg) {
    //TODO
  }

};

}

#endif
