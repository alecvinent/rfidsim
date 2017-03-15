#ifndef RFIDSIMPP_RADIO_BER_CHANNEL_MODELS_H_
#define RFIDSIMPP_RADIO_BER_CHANNEL_MODELS_H_

#include <omnetpp.h>

namespace rfidsim {

class ChannelModel : public omnetpp::cSimpleModule {
 public:
  virtual ~ChannelModel() {}

  virtual double getBER(double snr) = 0;
};

class ConstantChannel : public ChannelModel {
 public:
  virtual ~ConstantChannel() {}

  virtual double getBER(double snr);
 protected:
  virtual void initialize();

 private:
  double ber = 0.0;
};

}

#endif
