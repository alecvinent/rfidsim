#ifndef RADIO_ANTENNA_SIGNALS_H_
#define RADIO_ANTENNA_SIGNALS_H_

#include <omnetpp.h>
#include <common/units.h>

namespace rfidsim {

class Antenna;

/**
 * The signal indicating that the field originated by the given peer antenna
 * is down.
 */
class ReceivedFieldOff: public omnetpp::cObject, omnetpp::noncopyable {
 public:
  static const char *NAME;
  int receiver_device_id = -1;
  int peer_device_id = -1;

  ReceivedFieldOff() {}
  ReceivedFieldOff(int receiver_device_id, int peer_device_id)
    : receiver_device_id(receiver_device_id), peer_device_id(peer_device_id) {}

  virtual std::string info() const;
};

/**
 * The signal indicating that antenna received field after it was turned off.
 */
class ReceivedFieldOn : public omnetpp::cObject, omnetpp::noncopyable {
 public:
  static const char *NAME;
  int device_id = -1;
  Antenna *peer_antenna;
  Power power = Power::ZERO;

  ReceivedFieldOn() {}
  ReceivedFieldOn(int device_id, Antenna *peer, const Power& power)
    : device_id(device_id), peer_antenna(peer), power(power) {}

  virtual std::string info() const;
};

/**
 * The signal indicating that antenna received field is updated. Field `power`
 * provides the power value at the antenna port. It is expected that antenna
 * computed this value taking into account cable loss.
 */
class ReceivedFieldUpdated : public omnetpp::cObject, omnetpp::noncopyable {
 public:
  static const char *NAME;
  int device_id = -1;
  Antenna *peer_antenna;
  Power power = Power::ZERO;

  ReceivedFieldUpdated() {}
  ReceivedFieldUpdated(int device_id, Antenna *peer, const Power& power)
    : device_id(device_id), peer_antenna(peer), power(power) {}

  virtual std::string info() const;
};


/**
 * The signal is emitted by the antenna to inform the medium that the
 * radiated field was turned off. Upon receiving it, transceivers and antennas
 * should forget the peer device's antenna.
 */
class RadiatedFieldOff : public omnetpp::cObject, omnetpp::noncopyable {
 public:
  static const char *NAME;

  int device_id = -1;

  RadiatedFieldOff() {}
  RadiatedFieldOff(int device_id) : device_id(device_id) {}

  virtual std::string info() const;
};

/**
 * The signal is emitted by the antenna to inform the peer antennas and
 * transceivers that the radiated field was turned off.
 */
class RadiatedFieldOn : public omnetpp::cObject, omnetpp::noncopyable {
 public:
  static const char *NAME;

  int device_id = -1;
  Antenna *peer_antenna = nullptr;
  Power radiated_power = Power::ZERO;

  RadiatedFieldOn() {}
  RadiatedFieldOn(int device_id, Antenna *antenna, const Power& power)
    : device_id(device_id), peer_antenna(antenna), radiated_power(power) {}

  virtual std::string info() const;
};

/**
 * The signal is emitted by the antenna to inform the peers that radiated field
 * power or antenna (including antenna movement and switching) was updated,
 * while not turned off.
 */
class RadiatedFieldUpdated : public omnetpp::cObject, omnetpp::noncopyable {
 public:
  static const char *NAME;

  int device_id = -1;
  Antenna *peer_antenna = nullptr;
  Power radiated_power = Power::ZERO;

  RadiatedFieldUpdated() {}
  RadiatedFieldUpdated(int device_id, Antenna *antenna, const Power& power)
    : device_id(device_id), peer_antenna(antenna), radiated_power(power) {}

  virtual std::string info() const;
};

/**
 * The signal is emitted by the Antenna Switching Unit to inform that the
 * antenna with specific index must be activated.
 */
class SwitchAntenna : public omnetpp::cObject, omnetpp::noncopyable {
public:
  static const char *NAME;
  int device_id;
  int index;

  SwitchAntenna() {}
  SwitchAntenna(int device_id, int index)
    : device_id(device_id), index(index) {}

  virtual std::string info() const;
};


/**
 * The signal is emitted to inform neighbor units (generally, inside the device)
 * that antenna was switched.
 */
class AntennaActivated : public omnetpp::cObject, omnetpp::noncopyable {
public:
  static const char *NAME;
  int device_id;
  Antenna *antenna;
  int index;

  AntennaActivated() {}
  AntennaActivated(int device_id, Antenna *antenna)
    : device_id(device_id), antenna(antenna) {}

  virtual std::string info() const;
};

}

#endif
