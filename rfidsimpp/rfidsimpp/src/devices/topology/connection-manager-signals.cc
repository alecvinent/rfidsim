#include <devices/topology/connection-manager-signals.h>

using namespace omnetpp;

namespace rfidsim {

Register_Class(ConnectionCreated);
Register_Class(ConnectionLost);

const char *ConnectionCreated::NAME = "ConnectionCreated";
const char *ConnectionLost::NAME = "ConnectionLost";

std::string ConnectionCreated::info() const
{
  static char buf[256];
  snprintf(buf, sizeof(buf),
           "%s {ReaderID=%d, TagID=%d, Reader=<%p>, Tag=<%p>}",
           NAME, reader_id, tag_id, reader, tag);
  return buf;
}

std::string ConnectionLost::info() const
{
  static char buf[128];
  snprintf(buf, sizeof(buf), "%s {ReaderID=%d, TagID=%d}", NAME, reader_id,
           tag_id);
  return buf;
}

}
