#include <radio/transceivers/active-transceiver.h>

using namespace omnetpp;

namespace rfidsim {

//
//===========================================================================
// ActiveTransmitter
//===========================================================================
//
Define_Module(ActiveTransmitter);

AirFrame *ActiveTransmitter::buildAirFrame(SendDataReq *request) const
{
  SendCommandReq *send_command_req = check_and_cast<SendCommandReq*>(request);
  auto preamble = send_command_req->getPreamble();
  auto command = check_and_cast<epcstd::Command*>(
          send_command_req->getEncapsulatedPacket());

  ReaderAirFrame *frame = new ReaderAirFrame;
  frame->setDeviceID(getDeviceID());
  frame->setBandwidth(getBandwidth());
  frame->setPreambleDuration(preamble.getDuration());
  frame->setBodyDuration(frame->getBodyDuration());
  frame->setPreamble(preamble);
  frame->setModulationLoss(Decibel::ZERO);
  frame->setBodyBitLength(request->getBodyBitLength());
  frame->encapsulate(command);

  return frame;
}

//
//===========================================================================
// ActiveReceiver
//===========================================================================
//
Define_Module(ActiveReceiver);

double ActiveReceiver::getFrameSNR(AirFrame *frame, double snr) const
{
  auto tag_frame = check_and_cast<TagAirFrame*>(frame);
  auto preamble = tag_frame->getPreamble();

  double preamble_duration = preamble.getDuration().dbl();
  double bandwidth = frame->getBandwidth();
  double spb = epcstd::getSymbolsPerBit(preamble.getTagEncoding());
  double symbol_duration = preamble.getSymbolDuration().dbl();

  double sync_angle = 1.0 / sqrt(snr * preamble_duration * bandwidth);
  double x = spb * snr * symbol_duration * bandwidth * cos(sync_angle);

  return x * x;
}

RecvDataInd *ActiveReceiver::buildRecvDataInd(AirFrame *frame) const
{
  auto tag_frame = check_and_cast<TagAirFrame*>(frame);
  auto preamble = tag_frame->getPreamble();
  auto reply = check_and_cast<epcstd::Reply*>(frame->getEncapsulatedPacket());

  auto ind = new RecvReplyInd;
  ind->setReplyKind(tag_frame->getReplyKind());
  ind->setPreamble(preamble);
  ind->encapsulate(reply);

  return ind;
}

}
