#ifndef RFIDSIMPP_RADIO_TRANSCEIVERS_PASSIVE_TRANSCEIVER_H_
#define RFIDSIMPP_RADIO_TRANSCEIVERS_PASSIVE_TRANSCEIVER_H_

#include <radio/transceivers/transceiver-base.h>

namespace rfidsim {

class PassiveTransmitter : public Transmitter {
 public:
  virtual ~PassiveTransmitter() {}

  const Decibel& getModulationLoss() const { return modulation_loss; }
  void setModulationLoss(const Decibel& loss) { modulation_loss = loss; }

 protected:
  virtual void initialize();
  virtual AirFrame *buildAirFrame(SendDataReq *request) const;

 private:
  Decibel modulation_loss = Decibel::ZERO;
};


class PassiveReceiver : public Receiver {
 public:
  virtual ~PassiveReceiver() {}

 protected:
  virtual double getFrameSNR(AirFrame *frame, double raw_snr) const;
  virtual RecvDataInd *buildRecvDataInd(AirFrame *frame) const;
};

}

#endif
