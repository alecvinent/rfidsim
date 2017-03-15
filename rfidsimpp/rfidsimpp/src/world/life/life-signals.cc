#include <world/life/life-signals.h>

using namespace omnetpp;

namespace rfidsim {

Register_Class(DestroyThing);

const char *DestroyThing::NAME = "DestroyThing";

std::string DestroyThing::info() const
{
  static char buf[128];
  snprintf(buf, sizeof(buf), "%s {ThingID=%d}", NAME, thing_id);
  return buf;
}

}
