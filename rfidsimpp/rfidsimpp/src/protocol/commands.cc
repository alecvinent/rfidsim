#include <protocol/commands.h>

using namespace omnetpp;

namespace rfidsim {

const simtime_t ReaderPreamble::DEFAULT_DELIM = 12.5e-6;

ReaderPreamble ReaderPreamble::createPreamble(simtime_t tari, simtime_t rtcal,
    simtime_t trcal, simtime_t delim) {
  ReaderPreamble ret;
  ret.setType(ReaderPreamble::PREAMBLE);
  ret.setTari(tari);
  ret.setRTcal(rtcal);
  ret.setTRcal(trcal);
  ret.setDelim(delim);
  return ret;
}

ReaderPreamble ReaderPreamble::createSync(simtime_t tari, simtime_t rtcal,
    simtime_t delim) {
  ReaderPreamble ret;
  ret.setType(ReaderPreamble::SYNC);
  ret.setTari(tari);
  ret.setRTcal(rtcal);
  ret.setDelim(delim);
  return ret;
}

}
