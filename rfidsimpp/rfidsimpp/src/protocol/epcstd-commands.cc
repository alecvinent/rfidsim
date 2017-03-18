#include <protocol/epcstd-commands.h>

using namespace omnetpp;

namespace rfidsim {
namespace epcstd {

const simtime_t ReaderPreamble::DEFAULT_DELIM = 12.5e-6;

//
//===========================================================================
// ReaderPreamble
//===========================================================================
//

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

//
/////////////////////////////////////////////////////////////////////////////
// Commands implementation
/////////////////////////////////////////////////////////////////////////////
//
Register_Class(Query);
Register_Class(QueryRep);
Register_Class(Ack);
Register_Class(ReqRN);
Register_Class(Read);

//
//===========================================================================
// Query
//===========================================================================
//
const char *Query::NAME = "Query";

std::string Query::info() const
{
  static char buf[512];
  snprintf(buf, sizeof(buf), "%s {DR=%s, M=%s, TRext=%d, Sel=%s, Session=%s"
          "Target=%s, q=%X, CRC=%X}", NAME,
          str(static_cast<DivideRatio>(getDR())),
          str(static_cast<TagEncoding>(getM())),
          (getTRext() ? 1 : 0),
          str(static_cast<Sel>(getSel())),
          str(static_cast<Session>(getSession())),
          str(static_cast<InventoryFlag>(getTarget())),
          getQ(), getCRC5());
  return buf;
}


//
//===========================================================================
// QueryRep
//===========================================================================
//
const char *QueryRep::NAME = "QueryRep";

std::string QueryRep::info() const
{
  static char buf[512];
  snprintf(buf, sizeof(buf), "%s {Session=%s}", NAME,
          str(static_cast<Session>(getSession())));
  return buf;
}


//
//===========================================================================
// ACK
//===========================================================================
//
const char *Ack::NAME = "ACK";

std::string Ack::info() const
{
  static char buf[512];
  snprintf(buf, sizeof(buf), "%s {RN=%4X}", NAME, getRN());
  return buf;
}


//
//===========================================================================
// ReqRN
//===========================================================================
//
const char *ReqRN::NAME = "ReqRN";

std::string ReqRN::info() const
{
  static char buf[512];
  snprintf(buf, sizeof(buf), "%s {RN=%4X, CRC=%4X}", NAME, getRN(), getCRC16());
  return buf;
}


//
//===========================================================================
// Read
//===========================================================================
//
const char *Read::NAME = "Read";

std::string Read::info() const
{
  static char buf[512];
  snprintf(buf, sizeof(buf),
          "%s {Bank=%s, WordPtr=%X, WordCount=%d, RN=%4X, CRC=%4X}", NAME,
          str(static_cast<MemoryBank>(getBank())), getWordPtr(), getWordCount(),
          getRN(), getCRC16());
  return buf;
}


}}
