#ifndef RFIDSIMPP_RADIO_ANTENNA_SWITCHING_UNIT_H_
#define RFIDSIMPP_RADIO_ANTENNA_SWITCHING_UNIT_H_

#include <omnetpp.h>
#include <radio/antenna-signals.h>
#include <power/power-unit-signals.h>

namespace rfidsim {

class Antenna;

class AntennaSwitchingUnit : public omnetpp::cSimpleModule,
                             public omnetpp::cListener {
 public:
  static omnetpp::simsignal_t SWITCH_ANTENNA_SIGNAL_ID;

  virtual ~AntennaSwitchingUnit();

 protected:
  virtual int numInitStages() const;
  virtual void initialize(int stage);

  virtual void receiveSignal(
          omnetpp::cComponent *source, omnetpp::simsignal_t signal_id,
          omnetpp::cObject *obj, omnetpp::cObject *details);

  virtual void handleMessage(omnetpp::cMessage *msg);

  virtual void processTimer();
  virtual void processPowerOn(const PowerOn& upd);
  virtual void processPowerOff(const PowerOff& upd);

  void switchAntenna(int next_index = -1);

 private:
  bool switch_only_when_powered = false;
  omnetpp::simtime_t interval = omnetpp::SimTime::ZERO;
  int device_id;

  int current_index = -1;
  int num_antennas = 0;
  std::vector<Antenna*> antennas;

  bool is_powered = false;
  omnetpp::cMessage *timer = nullptr;

  std::map<omnetpp::simsignal_t, omnetpp::cModule*> subscriptions;
};

}

#endif /* RADIO_ANTENNA_SWITCHING_UNIT_H_ */
