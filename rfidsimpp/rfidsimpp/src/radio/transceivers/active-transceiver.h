#ifndef RFIDSIMPP_RADIO_TRANSCEIVERS_ACTIVE_TRANSCEIVER_H_
#define RFIDSIMPP_RADIO_TRANSCEIVERS_ACTIVE_TRANSCEIVER_H_

#include <radio/transceivers/transceiver-base.h>

namespace rfidsim {

class ActiveTransmitter : public Transmitter {
 public:
  virtual ~ActiveTransmitter() {}

 protected:
  virtual AirFrame *buildAirFrame(SendDataReq *request) const;
};

}

#endif
