#ifndef RFIDSIMPP_ANTENNA_ANTENNA_H_
#define RFIDSIMPP_ANTENNA_ANTENNA_H_

#include <omnetpp.h>
#include <common/units.h>
#include <common/geometry.h>
#include <common/optional.hpp>
#include <world/mobility/mobility-units.h>
#include <radio/antenna-signals.h>
#include <power/power-unit-signals.h>

namespace rfidsim {

class Medium;

class Antenna : public StationaryMobilityUnit {
 public:
  static omnetpp::simsignal_t RECEIVED_FIELD_OFF_SIGNAL_ID;
  static omnetpp::simsignal_t RECEIVED_FIELD_ON_SIGNAL_ID;
  static omnetpp::simsignal_t RECEIVED_FIELD_UPDATED_SIGNAL_ID;
  static omnetpp::simsignal_t RADIATED_FIELD_OFF_SIGNAL_ID;
  static omnetpp::simsignal_t RADIATED_FIELD_ON_SIGNAL_ID;
  static omnetpp::simsignal_t RADIATED_FIELD_UPDATED_SIGNAL_ID;
  static omnetpp::simsignal_t ANTENNA_ACTIVATED_SIGNAL_ID;

  enum Polarization {
    HORIZONTAL, VERTICAL, CIRCULAR
  };
  static const char *str(Polarization polarization);
  static Decibel getPolarizationLoss(Polarization txp, Polarization rxp);

  virtual ~Antenna();

  int getDeviceID() const { return device_id; }
  int getIndex() const { return index; }
  const Decibel& getGain() const { return gain; }
  const Decibel& getCableLoss() const { return cable_loss; }
  const bool isActive() const { return is_active; }
  bool isPowered() const { return is_powered; }
  const Power& getPower() const { return power; }
  Polarization getPolarization() const { return polarization; }

  void setIndex(int index) { this->index = index; }

  virtual const char *getAntennaTypeName() const = 0;
  virtual std::string getAntennaName() const;

  virtual Decibel getLoss(double cos_fwd, double cos_right) const = 0;
  virtual Decibel getRxLoss(const Vector3& direction) const;
  virtual Decibel getTxLoss(const Vector3& direction) const;

  virtual Medium *getMedium() const { return medium; }

 protected:
  virtual int numInitStages() const;
  virtual void initialize(int stage);

  virtual void receiveSignal(
          omnetpp::cComponent *source, omnetpp::simsignal_t signal_id,
          omnetpp::cObject *obj, omnetpp::cObject *details);

  virtual void processPositionUpdate();
  virtual void processRadiatedFieldOff(const RadiatedFieldOff& upd);
  virtual void processRadiatedFieldOn(const RadiatedFieldOn& upd);
  virtual void processRadiatedFieldUpdated(const RadiatedFieldUpdated& upd);
  virtual void processPowerOff(const PowerOff& upd);
  virtual void processPowerOn(const PowerOn& upd);
  virtual void processPowerUpdated(const PowerUpdated& upd);
  virtual void processSwitchAntenna(const SwitchAntenna& upd);

 private:
  int device_id = -1;
  int index = 0;
  Decibel gain = Decibel::ZERO;
  Decibel cable_loss = Decibel::ZERO;
  Polarization polarization;

  bool is_active = false;
  bool is_powered = false;
  Power power = Power::ZERO;

  Medium *medium = nullptr;
  std::map<omnetpp::simsignal_t, omnetpp::cModule*> subscriptions;
};

}

#endif
