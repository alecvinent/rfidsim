#include <protocol/responses.h>

using namespace omnetpp;

namespace rfidsim {
namespace epcstd {

int TagPreamble::getBitLength() const {
  if (m_ == FM_0)
    return trext_ ? 18 : 6;
  else
    return trext_ ? 22 : 10;
}

std::string TagPreamble::info() const {
  static char buf[256];
  snprintf(buf, sizeof(buf), "{m=%s, dr=%s, trext=%d, trcal=%.3fus}",
      str(m_), str(dr_), trext_ ? 1 : 0,
          trcal_.dbl() * 1e6);
  return buf;
}

}}
