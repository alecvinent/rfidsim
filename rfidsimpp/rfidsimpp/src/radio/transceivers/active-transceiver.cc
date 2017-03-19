#include <radio/transceivers/active-transceiver.h>

using namespace omnetpp;

namespace rfidsim {

Define_Module(ActiveTransmitter);

//
//===========================================================================
// ActiveTransmitter
//===========================================================================
//

AirFrame *ActiveTransmitter::buildAirFrame(SendDataReq *request) const
{
  SendCommandReq *send_command_req = check_and_cast<SendCommandReq*>(request);
  auto preamble = send_command_req->getPreamble();
  auto command = check_and_cast<epcstd::Command*>(
          send_command_req->decapsulate());

  ReaderAirFrame *frame = new ReaderAirFrame;
  frame->setDeviceID(getDeviceID());
  frame->setBandwidth(getBandwidth());
  frame->setPreambleDuration(preamble.getDuration());
  frame->setBodyDuration(frame->getBodyDuration());
  frame->setPreamble(preamble);
  frame->setModulationLoss(Decibel::ZERO);
  frame->encapsulate(command);

  return frame;
}

}
