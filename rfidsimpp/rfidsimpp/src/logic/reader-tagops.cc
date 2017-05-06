#include <logic/reader-tagops.h>
#include <common/functions.h>

using namespace omnetpp;

namespace rfidsim {
namespace tagop {

const char *str(FailureCode code)
{
  switch (code) {
    case CHANNEL_ERROR: return "Channel Error";
    case NO_RESPONSE: return "No Response";
    default: throw cRuntimeError("unrecognized FailureCode = %d", code);
  }
}

const char *Acknowledge::NAME = "Acknowledge";
const char *ReadBank::NAME = "ReadBank";
const char *Success::NAME = "Success";
const char *Failure::NAME = "Failure";
const char *TagInfo::NAME = "TagInfo";
const char *Data::NAME = "Data";

std::string Acknowledge::info() const
{
  return NAME;
}

std::string ReadBank::info() const
{
  char buf[512];
  snprintf(buf, sizeof(buf), "%s {Bank=%s, WordPtr=%u, WordCnt=%u}", NAME,
           epcstd::str(static_cast<epcstd::MemoryBank>(getBank())),
           getWordPtr(), getWordCount());
  return buf;
}

std::string Success::info() const
{
  return NAME;
}

std::string Failure::info() const
{
  char buf[512];
  snprintf(buf, sizeof(buf), "%s {Code='%s'}", NAME,
           str(static_cast<FailureCode>(getCode())));
  return buf;
}

std::string TagInfo::info() const
{
  char buf[512];
  snprintf(buf, sizeof(buf), "%s {PC=%u, EPC=%s}", NAME, getPC(),
           ::rfidsim::toHexString(getEPC()).c_str());
  return buf;
}

std::string Data::info() const
{
  char buf[512];
  snprintf(buf, sizeof(buf), "%s {Data=%s}", NAME,
           ::rfidsim::toHexString(getData()).c_str());
  return buf;
}

}}
