#ifndef RFIDSIMPP_RADIO_BER_CHANNEL_MODELS_H_
#define RFIDSIMPP_RADIO_BER_CHANNEL_MODELS_H_

#include <omnetpp.h>

namespace rfidsim {

class ChannelModel : public omnetpp::cSimpleModule {
 public:
  virtual ~ChannelModel() {}

  virtual double getBER(double snr) = 0;
};

class ConstantBERChannelModel : public ChannelModel {
 public:
  virtual ~ConstantBERChannelModel() {}

  virtual double getBER(double snr) { return ber; }
 protected:
  virtual void initialize();

 private:
  double ber = 0.0;
};


class AWGNChannelModel : public ChannelModel {
 public:
  virtual ~AWGNChannelModel() {}
  virtual double getBER(double snr);
};


class RayleighChannelModel : public ChannelModel {
 public:
  virtual ~RayleighChannelModel() {}
  virtual double getBER(double snr);
};

}

#endif
