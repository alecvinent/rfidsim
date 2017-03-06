#include <protocol/data-types.h>

using namespace omnetpp;

namespace rfidsim {

const char *strTagEncoding(TagEncoding v) {
  switch (v) {
    case FM_0: return "FM0";
    case MILLER_2: return "M2";
    case MILLER_4: return "M4";
    case MILLER_8: return "M8";
    default:
      ASSERT2(false, "unexpected TagEncoding value");
  }
}

int getSymbolsPerBit(TagEncoding v) {
  switch (v) {
    case FM_0: return 1;
    case MILLER_2: return 2;
    case MILLER_4: return 4;
    case MILLER_8: return 8;
    default:
      ASSERT2(false, "unexpected TagEncoding value");
  }
}

const char *strDivideRatio(DivideRatio v) {
  switch (v) {
    case DR_8: return "8";
    case DR_64_3: return "64/3";
    default:
      ASSERT2(false, "unexpected DivideRation value");
  }
}

double getDivideRationValue(DivideRatio v) {
  switch (v) {
    case DR_8: return 8.0;
    case DR_64_3: return 64.0/3;
    default:
      ASSERT2(false, "unexpected DivideRation value");
  }
}

double getBLF(omnetpp::simtime_t trcal, DivideRatio dr) {
  return getDivideRationValue(dr) / trcal.dbl();
}

double getTagBitRate(omnetpp::simtime_t trcal, DivideRatio dr, TagEncoding m) {
  return getBLF(trcal, dr) / getSymbolsPerBit(m);
}

}
