#include <world/mobility/mobility-signals.h>

using namespace omnetpp;

namespace rfidsim {

Register_Class(PositionUpdated);
Register_Class(UpdatePosition);

const char *PositionUpdated::NAME = "PositionUpdated";
const char *UpdatePosition::NAME = "UpdatePosition";

}
