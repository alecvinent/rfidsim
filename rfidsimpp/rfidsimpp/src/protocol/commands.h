#ifndef RFIDSIMPP_PROTOCOL_COMMANDS_H_
#define RFIDSIMPP_PROTOCOL_COMMANDS_H_

#include <omnetpp.h>

namespace rfidsim {

class ReaderPreamble : public omnetpp::cOwnedObject {
public:
  static const omnetpp::simtime_t DEFAULT_DELIM;

  enum Type {
    PREAMBLE, SYNC
  };

  static ReaderPreamble createPreamble(omnetpp::simtime_t tari,
      omnetpp::simtime_t rtcal, omnetpp::simtime_t trcal,
      omnetpp::simtime_t delim = DEFAULT_DELIM);

  static ReaderPreamble createSync(omnetpp::simtime_t tari,
      omnetpp::simtime_t rtcal, omnetpp::simtime_t delim = DEFAULT_DELIM);

  ReaderPreamble() {}

  ReaderPreamble(const ReaderPreamble& other)
    : type_(other.type_), tari_(other.tari_), rtcal_(other.rtcal_),
      trcal_(other.trcal_), delim_(other.delim_) {}

  virtual ~ReaderPreamble() {}

  ReaderPreamble& operator=(const ReaderPreamble& rside) {
    type_ = rside.getType();
    tari_ = rside.getTari();
    rtcal_ = rside.getRTcal();
    trcal_ = rside.getTRcal();
    delim_ = rside.getDelim();
    return *this;
  }

  virtual ReaderPreamble *dup() const {
    return new ReaderPreamble(*this);
  }

  Type getType() const { return type_; }
  omnetpp::simtime_t getTari() const { return tari_; }
  omnetpp::simtime_t getRTcal() const { return rtcal_; }
  omnetpp::simtime_t getTRcal() const { return trcal_; }
  omnetpp::simtime_t getDelim() const { return delim_; }

  void setType(Type type) { type_ = type; }
  void setTari(omnetpp::simtime_t tari) { tari_ = tari; }
  void setRTcal(omnetpp::simtime_t rtcal) { rtcal_ = rtcal; }
  void setTRcal(omnetpp::simtime_t trcal) { trcal_ = trcal; }
  void setDelim(omnetpp::simtime_t delim) { delim_ = delim; }

  void setRTcalAsTariMul(double k) { rtcal_ = k * tari_; }
  void setTRcalAsRTcalMul(double k) { trcal_ = k * rtcal_; }

  omnetpp::simtime_t getData0() const { return tari_; }
  omnetpp::simtime_t getData1() const { return rtcal_ - tari_; }

  omnetpp::simtime_t getDuration() const {
    return delim_ + tari_ + rtcal_ +
        (type_ == PREAMBLE ? trcal_ : omnetpp::SimTime::ZERO);
  }

 private:
  Type type_ = SYNC;
  omnetpp::simtime_t tari_ = omnetpp::SimTime::ZERO;
  omnetpp::simtime_t rtcal_ = omnetpp::SimTime::ZERO;
  omnetpp::simtime_t trcal_ = omnetpp::SimTime::ZERO;
  omnetpp::simtime_t delim_ = DEFAULT_DELIM;
};

}

#endif
