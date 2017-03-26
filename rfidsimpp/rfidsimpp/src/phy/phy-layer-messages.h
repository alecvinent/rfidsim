#ifndef RFIDSIMPP_PHY_PHY_MESSAGES_H_
#define RFIDSIMPP_PHY_PHY_MESSAGES_H_

#include <omnetpp.h>
#include <phy/phy-layer-messages_m.h>

namespace rfidsim {

const char *str(PhyDataConfStatus status);

class PhyDataConf : public PhyDataConf_Base {
 public:
  static const char *NAME;

  PhyDataConf() : PhyDataConf_Base(NAME) {}
  PhyDataConf(const PhyDataConf& other) : PhyDataConf_Base(other) {}
  virtual ~PhyDataConf() {}

  virtual PhyDataConf *dup() const { return new PhyDataConf(*this); }

  PhyDataConf& operator=(const PhyDataConf& other) {
    PhyDataConf_Base::operator=(other);
    return *this;
  }

  virtual std::string info() const;
};

}

#endif
