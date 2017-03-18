#include <phy/tag-phy-layer.h>

using namespace omnetpp;

namespace rfidsim {

Define_Module(TagPhy);

TagPhy::~TagPhy()
{
  //TODO
}

void TagPhy::initialize()
{
  Phy::initialize();
  reset();
}

void TagPhy::handleMessage(cMessage *msg)
{
  if (dynamic_cast<epcstd::Reply*>(msg))
    processReply(static_cast<epcstd::Reply*>(msg));
  else
    Phy::handleMessage(msg);
}

void TagPhy::processPowerOn(const PowerOn& signal)
{
  //TODO
}

void TagPhy::processPowerOff(const PowerOff& signal)
{
  //TODO
}

void TagPhy::processTimeout(cMessage *msg)
{
  //TODO
}

void TagPhy::processRecvBeginInd(RecvBeginInd *msg)
{
  //TODO
}

void TagPhy::processRecvDataInd(RecvDataInd *msg)
{
  //TODO
}

void TagPhy::processRecvErrorInd(RecvErrorInd *msg)
{
  //TODO
}

void TagPhy::processReply(epcstd::Reply *msg)
{
  //TODO
}

void TagPhy::reset()
{
  encoding = nonstd::nullopt;
  divide_ratio = nonstd::nullopt;
  use_extended_preamble = nonstd::nullopt;
  blf = nonstd::nullopt;
  bitrate = nonstd::nullopt;
}

}
