#include <epcstd-base.h>

using namespace omnetpp;

namespace rfidsim {
namespace epcstd {

const char *str(CommandType v)
{
  switch (v)
  {
    case COMMAND_QUERY:
      return "Query";
    case COMMAND_QUERY_REP:
      return "QueryRep";
    case COMMAND_ACK:
      return "ACK";
    case COMMAND_REQ_RN:
      return "ReqRN";
    case COMMAND_READ:
      return "Read";
    default:
      throw cRuntimeError("unexpected CommandType = %d", v);
  }
}

const char *str(TagEncoding v)
{
  switch (v)
  {
    case FM_0:
      return "FM0";
    case MILLER_2:
      return "M2";
    case MILLER_4:
      return "M4";
    case MILLER_8:
      return "M8";
    default:
      throw cRuntimeError("unexpected TagEncoding = %d", v);
  }
}

const char *str(DivideRatio v)
{
  switch (v)
  {
    case DR_8:
      return "8";
    case DR_64_3:
      return "64/3";
    default:
      throw cRuntimeError("unexpected DivideRatio = %d", v);
  }
}

const char *str(Sel v)
{
  switch (v)
  {
    case SEL_ALL:
      return "ALL";
    case SEL_YES:
      return "SL";
    case SEL_NOT:
      return "~SL";
    default:
      throw cRuntimeError("unexpected Sel = %d", v);
  }
}

const char *str(Session v)
{
  switch (v)
  {
    case SESSION_0:
      return "S0";
    case SESSION_1:
      return "S1";
    case SESSION_2:
      return "S2";
    case SESSION_3:
      return "S3";
    default:
      throw cRuntimeError("unexpected Session = %d", v);
  }
}

const char *str(InventoryFlag v)
{
  switch (v)
  {
    case FLAG_A:
      return "A";
    case FLAG_B:
      return "B";
    default:
      throw cRuntimeError("unexpected InventoryFlag = %d", v);
  }
}

const char *str(MemoryBank v)
{
  switch (v)
  {
    case BANK_EPC:
      return "EPC";
    case BANK_TID:
      return "TID";
    case BANK_USER:
      return "USER";
    default:
      throw cRuntimeError("unexpected MemoryBank = %d", v);
  }
}

int getSymbolsPerBit(TagEncoding v)
{
  switch (v)
  {
    case FM_0:
      return 1;
    case MILLER_2:
      return 2;
    case MILLER_4:
      return 4;
    case MILLER_8:
      return 8;
    default:
      ASSERT2(false, "unexpected TagEncoding value");
  }
}

double getDivideRatioValue(DivideRatio v)
{
  switch (v)
  {
    case DR_8:
      return 8.0;
    case DR_64_3:
      return 64.0 / 3;
    default:
      ASSERT2(false, "unexpected DivideRation value");
  }
}

double getBLF(simtime_t trcal, DivideRatio dr)
{
  return getDivideRatioValue(dr) / trcal.dbl();
}

double getTagBitRate(simtime_t trcal, DivideRatio dr, TagEncoding m)
{
  return getBLF(trcal, dr) / getSymbolsPerBit(m);
}

simtime_t getTpri(double blf)
{
  return SimTime(1.0 / blf);
}

double getFrT(DivideRatio dr, simtime_t trcal, bool extended_temp)
{
  // Here we approximate what is given in table 6.9 of the EPC Class 1 Gen 2.
  // Very rough approximation.
  double t = trcal.dbl() * 1e6;
  if (dr == DR_64_3)
  {
    if (t <= 33.31) return 0.15;
    else if (t <= 66.69) return 0.22;
    else if (t <= 66.71) return extended_temp ? 0.15 : 0.10;
    else if (t <= 83.29) return extended_temp ? 0.15 : 0.12;
    else if (t <= 83.31) return 0.10;
    else if (t <= 133.30) return extended_temp ? 0.12 : 0.10;
    else if (t <= 200.00) return 0.07;
    else return 0.05;
  }
  else if (dr == DR_8)
  {
    if (t <= 24.99) return 0.19;
    else if (t <= 25.01) return extended_temp ? 0.15 : 0.10;
    else if (t <= 31.24) return extended_temp ? 0.15 : 0.12;
    else if (t <= 31.26) return 0.10;
    else if (t <= 49.99) return 0.10;
    else if (t <= 50.01) return 0.07;
    else if (t <= 75.00) return 0.07;
    else return 0.04;
  }
  else
  {
    throw cRuntimeError("unrecognized DivideRation = %d", dr);
  }
}

unsigned getSessionIndex(Session session)
{
  switch (session)
  {
    case SESSION_0:
      return 0;
    case SESSION_1:
      return 1;
    case SESSION_2:
      return 2;
    case SESSION_3:
      return 3;
    default:
      throw cRuntimeError("unrecognized Session = %d", session);
  }
}

Session getSessionFromIndex(unsigned index)
{
  switch (index)
  {
    case 0:
      return SESSION_0;
    case 1:
      return SESSION_1;
    case 2:
      return SESSION_2;
    case 3:
      return SESSION_3;
    default:
      throw cRuntimeError("session index = %u out-of-bounds", index);
  }
}

simtime_t LinkTimingHelper::getT(int k, Type type) const
{
  switch (type)
  {
    case MIN: return getMinT(k);
    case MAX: return getMaxT(k);
    case NOMINAL: return getNominalT(k);
    default: throw cRuntimeError("unrecognized timeout type = %d", type);
  }
}

simtime_t LinkTimingHelper::getMinT(int k) const
{
  if (k == 1 || k == 5 || k == 6)
  {
    return SimTime(std::max(rtcal->dbl(), 10 * t_pri->dbl()) * (1 - *frt)
                   - 2e-6);
  }
  else if (k == 2)
  {
    return SimTime(3.0 * this->t_pri->dbl());
  }
  else if (k == 3)
  {
    return SimTime(0.0 * this->t_pri->dbl());
  }
  else if (k == 4)
  {
    return SimTime(20.0 * this->rtcal->dbl());
  }
  else if (k == 7)
  {
    return SimTime(std::max(250e-6, getMaxT(2).dbl()));
  }
  else
  {
    throw cRuntimeError("timeout index must be 1..7, but %d found", k);
  }
}

simtime_t LinkTimingHelper::getMaxT(int k) const
{
  if (k == 1)
  {
    return SimTime(std::max(rtcal->dbl(), 10 * t_pri->dbl()) * (1 + *frt)
                   + 2e-6);
  }
  else if (k == 2)
  {
    return SimTime(20.0 * this->t_pri->dbl());
  }
  else if (k >= 5 && k <= 7)
  {
    return SimTime(20e-3);
  }
  else
  {
    throw cRuntimeError("only timeouts 1,2,5,6,7 have max value, %d found", k);
  }
}

simtime_t LinkTimingHelper::getNominalT(int k) const
{
  if (k == 1)
  {
    return SimTime(std::max(rtcal->dbl(), 10.0 * t_pri->dbl()));
  }
  else
  {
    throw cRuntimeError("only timeout 1 has nominal value, %d found", k);
  }
}


}}
