#include <phy/reader-phy.h>

using namespace omnetpp;

namespace rfidsim {

Define_Module(ReaderPhy);

ReaderPhy::~ReaderPhy()
{
  //TODO
}

void ReaderPhy::initialize()
{
  Phy::initialize();

  delimiter = SimTime(par("delimiter"));
  tari = SimTime(par("tari"));
  rtcal = SimTime(par("rtcal"));
  trcal = SimTime(par("trcal"));
}

void ReaderPhy::handleMessage(cMessage *msg)
{
  if (dynamic_cast<epcstd::Command*>(msg))
    processCommand(static_cast<epcstd::Command*>(msg));
  else
    Phy::handleMessage(msg);
}

void ReaderPhy::processPowerOn(const PowerOn& signal)
{
  //TODO
}

void ReaderPhy::processPowerOff(const PowerOff& signal)
{
  //TODO
}

void ReaderPhy::processTimeout(cMessage *msg)
{
  //TODO
}

void ReaderPhy::processRecvBeginInd(RecvBeginInd *msg)
{
  //TODO
}

void ReaderPhy::processRecvDataInd(RecvDataInd *msg)
{
  //TODO
}

void ReaderPhy::processRecvErrorInd(RecvErrorInd *msg)
{
  //TODO
}

void ReaderPhy::processCommand(epcstd::Command *msg)
{
  //TODO
}

}
