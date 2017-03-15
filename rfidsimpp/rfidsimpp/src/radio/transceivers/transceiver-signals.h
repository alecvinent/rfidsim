#ifndef RFIDSIMPP_RADIO_TRANSCEIVERS_TRANSCEIVER_SIGNALS_H_
#define RFIDSIMPP_RADIO_TRANSCEIVERS_TRANSCEIVER_SIGNALS_H_

#include <omnetpp.h>

namespace rfidsim {

class TxStart : public omnetpp::cObject, omnetpp::noncopyable {
public:
  static const char *NAME;
  int device_id = -1;

  TxStart() {}
  TxStart(int device_id) : device_id(device_id) {}
  virtual ~TxStart() {}

  virtual std::string info() const;
};

class TxFinish : public omnetpp::cObject, omnetpp::noncopyable {
public:
  static const char *NAME;
  int device_id = -1;

  TxFinish() {}
  TxFinish(int device_id) : device_id(device_id) {}
  virtual ~TxFinish() {}

  virtual std::string info() const;
};

}

#endif
