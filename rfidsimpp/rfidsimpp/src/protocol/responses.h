#ifndef RFIDSIMPP_PROTOCOL_RESPONSES_H_
#define RFIDSIMPP_PROTOCOL_RESPONSES_H_

#include <epcstd-data-types.h>
#include <omnetpp.h>

namespace rfidsim {
namespace epcstd {

class TagPreamble : public omnetpp::cOwnedObject {
 public:
  TagPreamble() {}

  TagPreamble(TagEncoding m, DivideRatio dr, bool trext,
      omnetpp::simtime_t trcal)
    : m_(m), dr_(dr), trext_(trext), trcal_(trcal) {}

  TagPreamble(const TagPreamble& other)
    : m_(other.m_), dr_(other.dr_), trext_(other.trext_), trcal_(other.trcal_)
  {}

  virtual ~TagPreamble() {}

  TagEncoding getTagEncoding() const { return m_; }
  DivideRatio getDivideRatio() const { return dr_; }
  bool getTRext() const { return trext_; }
  omnetpp::simtime_t getTRcal() const { return trcal_; }

  void setTagEncoding(TagEncoding m) { m_ = m; }
  void setDivideRatio(DivideRatio dr) { dr_ = dr; }
  void setTRext(bool trext) { trext_ = trext; }
  void setTRcal(omnetpp::simtime_t trcal) { trcal_ = trcal; }

  int getBitLength() const;

  omnetpp::simtime_t getDuration() const {
    return getBitLength() * getTagBitRate(trcal_, dr_, m_);
  }

  virtual std::string info() const;

 private:
  TagEncoding m_;
  DivideRatio dr_;
  bool trext_;
  omnetpp::simtime_t trcal_;
};

}}

#endif
