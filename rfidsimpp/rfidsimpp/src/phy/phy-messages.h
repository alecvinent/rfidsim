#ifndef RFIDSIMPP_PHY_PHY_MESSAGES_H_
#define RFIDSIMPP_PHY_PHY_MESSAGES_H_

#include <omnetpp.h>
#include <phy/phy-messages_m.h>

namespace rfidsim {

const char *str(PhyResponseType type);

class ReplyTimeoutInd : public ReplyTimeoutInd_Base {
 public:
  static const char *NAME;

  ReplyTimeoutInd() : ReplyTimeoutInd_Base(NAME) {}
  ReplyTimeoutInd(const ReplyTimeoutInd& other) : ReplyTimeoutInd_Base(other) {}
  virtual ~ReplyTimeoutInd() {}

  virtual ReplyTimeoutInd *dup() const { return new ReplyTimeoutInd(*this); }

  ReplyTimeoutInd& operator=(const ReplyTimeoutInd& other) {
    ReplyTimeoutInd_Base::operator=(other);
    return *this;
  }

  virtual std::string info() const;
};

}

#endif
