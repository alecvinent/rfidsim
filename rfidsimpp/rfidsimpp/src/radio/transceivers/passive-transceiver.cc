#include <radio/transceivers/passive-transceiver.h>

using namespace omnetpp;

namespace rfidsim {

//
//===========================================================================
// PassiveTransmitter
//===========================================================================
//
Define_Module(PassiveTransmitter);

void PassiveTransmitter::initialize()
{
  Transmitter::initialize();
  modulation_loss = Decibel(par("modulationLoss").doubleValue());
}


AirFrame *PassiveTransmitter::buildAirFrame(SendDataReq *request) const
{
  SendReplyReq *send_reply_req = check_and_cast<SendReplyReq*>(request);
  auto preamble = send_reply_req->getPreamble();
  auto reply = check_and_cast<epcstd::Reply*>(
          send_reply_req->getEncapsulatedPacket());

  auto frame = new TagAirFrame;
  frame->setDeviceID(getDeviceID());
  frame->setBandwidth(getBandwidth());
  frame->setPreambleDuration(preamble.getDuration());
  frame->setBodyDuration(frame->getBodyDuration());
  frame->setPreamble(preamble);
  frame->setModulationLoss(modulation_loss);
  frame->setBodyBitLength(request->getBodyBitLength());
  frame->encapsulate(reply);

  return frame;
}

//
//===========================================================================
// ActiveReceiver
//===========================================================================
//
Define_Module(PassiveReceiver);

double PassiveReceiver::getFrameSNR(AirFrame *frame, double snr) const
{
  return snr;
}

RecvDataInd *PassiveReceiver::buildRecvDataInd(AirFrame *frame) const
{
  auto reader_frame = check_and_cast<ReaderAirFrame*>(frame);
  auto preamble = reader_frame->getPreamble();
  auto command = check_and_cast<epcstd::Command*>(
          frame->getEncapsulatedPacket());

  auto ind = new RecvCommandInd;
  ind->setCommandKind(reader_frame->getCommandKind());
  ind->setPreamble(preamble);
  ind->encapsulate(command);

  return ind;
}

}
