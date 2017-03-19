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


class ActiveReceiver : public Receiver {
 public:
  virtual ~ActiveReceiver() {}

 protected:
  virtual double getFrameSNR(AirFrame *frame, double raw_snr) const;
  virtual RecvDataInd *buildRecvDataInd(AirFrame *frame) const;
};

}

#endif
