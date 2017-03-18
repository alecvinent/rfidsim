#include <epcstd-data-types.h>

using namespace omnetpp;

namespace rfidsim {
namespace epcstd {

const char *str(CommandType v)
{
  switch (v)
  {
    case COMMAND_QUERY: return "Query";
    case COMMAND_QUERY_REP: return "QueryRep";
    case COMMAND_ACK: return "ACK";
    case COMMAND_REQ_RN: return "ReqRN";
    case COMMAND_READ: return "Read";
    default: throw cRuntimeError("unexpected CommandType = %d", v);
  }
}

const char *str(TagEncoding v)
{
  switch (v) {
    case FM_0: return "FM0";
    case MILLER_2: return "M2";
    case MILLER_4: return "M4";
    case MILLER_8: return "M8";
    default: throw cRuntimeError("unexpected TagEncoding = %d", v);
  }
}

const char *str(DivideRatio v)
{
  switch (v) {
    case DR_8: return "8";
    case DR_64_3: return "64/3";
    default: throw cRuntimeError("unexpected DivideRatio = %d", v);
  }
}

const char *str(Sel v)
{
  switch (v) {
    case SEL_ALL: return "ALL";
    case SEL_YES: return "SL";
    case SEL_NOT: return "~SL";
    default: throw cRuntimeError("unexpected Sel = %d", v);
  }
}

const char *str(Session v)
{
  switch (v) {
    case SESSION_0: return "S0";
    case SESSION_1: return "S1";
    case SESSION_2: return "S2";
    case SESSION_3: return "S3";
    default: throw cRuntimeError("unexpected Session = %d", v);
  }
}

const char *str(InventoryFlag v)
{
  switch (v) {
    case FLAG_A: return "A";
    case FLAG_B: return "B";
    default: throw cRuntimeError("unexpected InventoryFlag = %d", v);
  }
}

const char *str(MemoryBank v)
{
  switch (v) {
    case BANK_EPC: return "EPC";
    case BANK_TID: return "TID";
    case BANK_USER: return "USER";
    default: throw cRuntimeError("unexpected MemoryBank = %d", v);
  }
}

int getSymbolsPerBit(TagEncoding v)
{
  switch (v) {
    case FM_0: return 1;
    case MILLER_2: return 2;
    case MILLER_4: return 4;
    case MILLER_8: return 8;
    default:
      ASSERT2(false, "unexpected TagEncoding value");
  }
}

double getDivideRatioValue(DivideRatio v)
{
  switch (v) {
    case DR_8: return 8.0;
    case DR_64_3: return 64.0/3;
    default:
      ASSERT2(false, "unexpected DivideRation value");
  }
}

double getBLF(omnetpp::simtime_t trcal, DivideRatio dr)
{
  return getDivideRatioValue(dr) / trcal.dbl();
}

double getTagBitRate(omnetpp::simtime_t trcal, DivideRatio dr, TagEncoding m)
{
  return getBLF(trcal, dr) / getSymbolsPerBit(m);
}

}}
