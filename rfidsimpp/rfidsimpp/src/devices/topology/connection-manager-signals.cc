#include <devices/topology/connection-manager-signals.h>

using namespace omnetpp;

namespace rfidsim {

Register_Class(ConnectionCreated);
Register_Class(ConnectionLost);

const char *ConnectionCreated::NAME = "ConnectionCreated";
const char *ConnectionLost::NAME = "ConnectionLost";

inline std::string _strSignal(const char *name, int reader_id, int tag_id)
{
  static char buf[128];
  snprintf(buf, sizeof(buf), "%s {ReaderID=%d, TagID=%d}", name,
           reader_id, tag_id);
  return buf;
}

std::string ConnectionCreated::info() const
{
  return _strSignal(NAME, reader_id, tag_id);
}

std::string ConnectionLost::info() const
{
  return _strSignal(NAME, reader_id, tag_id);
}

}
