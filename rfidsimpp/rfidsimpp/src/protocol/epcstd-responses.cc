#include <protocol/epcstd-responses.h>
#include <common/functions.h>

using namespace omnetpp;

namespace rfidsim {
namespace epcstd {

//
//===========================================================================
// TagPreamble
//===========================================================================
//
int TagPreamble::getBitLength() const {
  if (m_ == FM_0)
    return trext_ ? 18 : 6;
  else
    return trext_ ? 22 : 10;
}

std::string TagPreamble::info() const {
  static char buf[256];
  snprintf(buf, sizeof(buf), "{M=%s, DR=%s, TRext=%d, TRcal=%.3fus}",
      str(m_), str(dr_), trext_ ? 1 : 0,
          trcal_.dbl() * 1e6);
  return buf;
}

//
/////////////////////////////////////////////////////////////////////////////
// Commands implementation
/////////////////////////////////////////////////////////////////////////////
//
Register_Class(QueryReply);
Register_Class(AckReply);
Register_Class(ReqRNReply);
Register_Class(ReadReply);

const char *str(ReplyKind kind) {
  switch (kind) {
    case KIND_QUERY_REPLY: return QueryReply::NAME;
    case KIND_ACK_REPLY: return AckReply::NAME;
    case KIND_REQ_RN_REPLY: return ReqRNReply::NAME;
    case KIND_READ_REPLY: return ReadReply::NAME;
    default: throw cRuntimeError("unrecognized ReplyKind = %d", kind);
  }
}

//
//===========================================================================
// QueryReply
//===========================================================================
//
const char *QueryReply::NAME = "QueryReply";

std::string QueryReply::info() const
{
  char buf[64];
  snprintf(buf, sizeof(buf), "%s {RN16=%4X}", NAME, getRN16());
  return buf;
}

//
//===========================================================================
// AckReply
//===========================================================================
//
const char *AckReply::NAME = "AckReply";

std::string AckReply::info() const
{
  char buf[256];
  snprintf(buf, sizeof(buf), "%s {PC=%4X, EPC=%s, CRC=%4X}",
           NAME, getPC(), rfidsim::toHexString(getEPC()).c_str(), getCRC());
  return buf;
}

//
//===========================================================================
// ReqRNReply
//===========================================================================
//
const char *ReqRNReply::NAME = "ReqRNReply";

std::string ReqRNReply::info() const
{
  char buf[64];
  snprintf(buf, sizeof(buf), "%s {RN=%4X, CRC=%4X}", NAME, getRN(), getCRC16());
  return buf;
}

//
//===========================================================================
// ReadReply
//===========================================================================
//
const char *ReadReply::NAME = "ReadReply";

std::string ReadReply::info() const
{
  char buf[256];
  snprintf(buf, sizeof(buf), "%s {Memory=%s, RN=%4X, CRC=%4X}", NAME,
           rfidsim::toHexString(getMem()).c_str(), getRN(), getCRC16());
  return buf;
}

}}
