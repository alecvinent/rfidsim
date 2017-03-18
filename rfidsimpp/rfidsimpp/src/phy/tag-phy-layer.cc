#include <phy/tag-phy-layer.h>

using namespace omnetpp;

namespace rfidsim {

Define_Module(TagPhyLayer);

TagPhyLayer::~TagPhyLayer()
{
  //TODO
}

void TagPhyLayer::initialize()
{
  PhyLayer::initialize();
  reset();
}

void TagPhyLayer::handleMessage(cMessage *msg)
{
  if (dynamic_cast<epcstd::Reply*>(msg))
    processReply(static_cast<epcstd::Reply*>(msg));
  else
    PhyLayer::handleMessage(msg);
}

void TagPhyLayer::processPowerOn(const PowerOn& signal)
{
  //TODO
}

void TagPhyLayer::processPowerOff(const PowerOff& signal)
{
  //TODO
}

void TagPhyLayer::processTimeout(cMessage *msg)
{
  //TODO
}

void TagPhyLayer::processRecvBeginInd(RecvBeginInd *msg)
{
  //TODO
}

void TagPhyLayer::processRecvDataInd(RecvDataInd *msg)
{
  //TODO
}

void TagPhyLayer::processRecvErrorInd(RecvErrorInd *msg)
{
  //TODO
}

void TagPhyLayer::processReply(epcstd::Reply *msg)
{
  //TODO
}

void TagPhyLayer::reset()
{
  encoding = nonstd::nullopt;
  divide_ratio = nonstd::nullopt;
  use_extended_preamble = nonstd::nullopt;
  blf = nonstd::nullopt;
  bitrate = nonstd::nullopt;
}

}
