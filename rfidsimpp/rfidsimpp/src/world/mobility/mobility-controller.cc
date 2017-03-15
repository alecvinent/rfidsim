#include <world/mobility/mobility-controller.h>
#include <world/mobility/mobility-signals.h>

using namespace omnetpp;

namespace rfidsim {

simsignal_t MobilityController::UPDATE_POSITION_SIGNAL_ID =
        registerSignal(UpdatePosition::NAME);

MobilityController::~MobilityController()
{
  cancelAndDelete(timer);
}

void MobilityController::initialize()
{
  timer = new cMessage("mobility-controller-timer");
  interval = par("interval").doubleValue();
  scheduleAt(simTime(), timer);
}

void MobilityController::handleMessage(omnetpp::cMessage *msg)
{
  if (msg->isSelfMessage()) {
    processTimer(msg);
  } else {
    throw cRuntimeError("unexpected message %s at MobilityController",
                        msg->getFullName());
  }
}

void MobilityController::processTimer(cMessage *timer)
{
  if (timer == this->timer)
  {
    UpdatePosition signal_;
    emit(UPDATE_POSITION_SIGNAL_ID, &signal_);
    scheduleAt(simTime() + interval, timer);
  }
}

}
