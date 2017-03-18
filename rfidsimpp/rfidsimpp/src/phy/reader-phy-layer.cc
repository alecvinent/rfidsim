#include <phy/reader-phy-layer.h>

using namespace omnetpp;

namespace rfidsim {

Define_Module(ReaderPhyLayer);

ReaderPhyLayer::~ReaderPhyLayer()
{
  //TODO
}

void ReaderPhyLayer::initialize()
{
  PhyLayer::initialize();

  delimiter = SimTime(par("delimiter"));
  tari = SimTime(par("tari"));
  rtcal = SimTime(par("rtcal"));
  trcal = SimTime(par("trcal"));
}

void ReaderPhyLayer::handleMessage(cMessage *msg)
{
  if (dynamic_cast<epcstd::Command*>(msg))
    processCommand(static_cast<epcstd::Command*>(msg));
  else
    PhyLayer::handleMessage(msg);
}

void ReaderPhyLayer::processPowerOn(const PowerOn& signal)
{
  //TODO
}

void ReaderPhyLayer::processPowerOff(const PowerOff& signal)
{
  //TODO
}

void ReaderPhyLayer::processTimeout(cMessage *msg)
{
  //TODO
}

void ReaderPhyLayer::processRecvBeginInd(RecvBeginInd *msg)
{
  //TODO
}

void ReaderPhyLayer::processRecvDataInd(RecvDataInd *msg)
{
  //TODO
}

void ReaderPhyLayer::processRecvErrorInd(RecvErrorInd *msg)
{
  //TODO
}

void ReaderPhyLayer::processCommand(epcstd::Command *msg)
{
  //TODO
}

}
