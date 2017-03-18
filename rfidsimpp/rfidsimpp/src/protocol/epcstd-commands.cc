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

const char *str(CommandKind kind)
{
  switch (kind) {
    case KIND_COMMAND_QUERY: return "Query";
    case KIND_COMMAND_QUERY_REP: return "QueryRep";
    case KIND_COMMAND_ACK: return "ACK";
    case KIND_COMMAND_REQ_RN: return "ReqRN";
    case KIND_COMMAND_READ: return "Read";
    default: throw cRuntimeError("unrecognized CommandKind = %d", kind);
  }
}

//
//===========================================================================
// Query
//===========================================================================
//
const char *Query::NAME = "Query";

std::string Query::info() const
{
  char buf[512];
  snprintf(buf, sizeof(buf),
           "%s {ReaderID=%d, DR=%s, M=%s, TRext=%d, Sel=%s, Session=%s"
           "Target=%s, q=%X, CRC=%X}", NAME, getReaderID(),
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
  char buf[512];
  snprintf(buf, sizeof(buf), "%s {ReaderID=%d, Session=%s}", NAME,
           getReaderID(), str(static_cast<Session>(getSession())));
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
  char buf[512];
  snprintf(buf, sizeof(buf), "%s {ReaderID=%d, RN=%4X}", NAME, getReaderID(),
           getRN());
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
  char buf[512];
  snprintf(buf, sizeof(buf), "%s {ReaderID=%d, RN=%4X, CRC=%4X}", NAME,
           getReaderID(), getRN(), getCRC16());
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
  char buf[512];
  snprintf(buf, sizeof(buf),
          "%s {ReaderID=%d, Bank=%s, WordPtr=%X, WordCount=%d, RN=%4X, "
          "CRC=%4X}", NAME, getReaderID(),
          str(static_cast<MemoryBank>(getBank())), getWordPtr(), getWordCount(),
          getRN(), getCRC16());
  return buf;
}


}}
