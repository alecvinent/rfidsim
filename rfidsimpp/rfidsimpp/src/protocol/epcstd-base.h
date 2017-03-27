#ifndef RFIDSIMPP_PROTOCOL_EPCSTD_BASE_H_
#define RFIDSIMPP_PROTOCOL_EPCSTD_BASE_H_

#include <protocol/epcstd-base_m.h>
#include <common/optional.hpp>

namespace rfidsim {
namespace epcstd {

const char *str(CommandType v);
const char *str(TagEncoding v);
const char *str(DivideRatio v);
const char *str(Sel v);
const char *str(Session v);
const char *str(InventoryFlag v);
const char *str(MemoryBank v);

int getSymbolsPerBit(TagEncoding v);
double getDivideRatioValue(DivideRatio v);
double getBLF(omnetpp::simtime_t trcal, DivideRatio dr);
double getTagBitRate(omnetpp::simtime_t trcal, DivideRatio dr, TagEncoding m);
omnetpp::simtime_t getTpri(double blf);

double getFrT(DivideRatio dr, omnetpp::simtime_t trcal,
              bool extended_temp = false);

unsigned getSessionIndex(Session session);
Session getSessionFromIndex(unsigned index);

class LinkTimingHelper {
 public:
  enum Type { MIN, MAX, NOMINAL };

  omnetpp::simtime_t getT(int k, Type type) const;
  omnetpp::simtime_t getMinT(int k) const;
  omnetpp::simtime_t getMaxT(int k) const;
  omnetpp::simtime_t getNominalT(int k) const;

  bool hasRTcal() const { return rtcal.has_value(); }
  bool hasTpri() const { return t_pri.has_value(); }
  bool hasFrT() const { return frt.has_value(); }

  omnetpp::simtime_t getRTcal() const { return *rtcal; }
  omnetpp::simtime_t getTpri() const { return *t_pri; }
  double getFrT() const { return *frt; }

  void setRTcal(omnetpp::simtime_t value) { rtcal = value; }
  void setTpri(omnetpp::simtime_t value) { t_pri = value; }
  void setFrT(double value) { frt = value; }

  void unsetRTcal() { rtcal = nonstd::nullopt; }
  void unsetTpri() { t_pri = nonstd::nullopt; }
  void unsetFrT() { frt = nonstd::nullopt; }

 private:
  nonstd::optional<omnetpp::simtime_t> rtcal = nonstd::nullopt;
  nonstd::optional<omnetpp::simtime_t> t_pri = nonstd::nullopt;
  nonstd::optional<double> frt = nonstd::nullopt;
};

}}

#endif
