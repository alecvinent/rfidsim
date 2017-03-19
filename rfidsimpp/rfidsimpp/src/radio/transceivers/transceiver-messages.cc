#include <radio/transceivers/transceiver-messages.h>

using namespace omnetpp;

namespace rfidsim {

const char *strRecvErrorType(RecvErrorType value)
{
  switch (value)
  {
    case RECV_ERROR_LOW_SNR: return "Decoding Error";
    case RECV_ERROR_COLLISION: return "Collision";
    default: throw cRuntimeError("unrecognized RecvErrorType = %d", value);
  }
}

Register_Class(SendCommandReq);
Register_Class(SendReplyReq);
Register_Class(RecvBeginInd);
Register_Class(RecvCommandInd);
Register_Class(RecvReplyInd);
Register_Class(RecvErrorInd);
Register_Class(ReaderAirFrame);
Register_Class(TagAirFrame);

const char *SendCommandReq::NAME = "SendCommandReq";
const char *SendReplyReq::NAME = "SendReplyReq";
const char *RecvBeginInd::NAME = "RecvBeginInd";
const char *RecvCommandInd::NAME = "RecvCommandInd";
const char *RecvReplyInd::NAME = "RecvReplyInd";
const char *RecvErrorInd::NAME = "RecvErrorInd";
const char *ReaderAirFrame::NAME = "ReaderAirFrame";
const char *TagAirFrame::NAME = "TagAirFrame";

std::string SendCommandReq::info() const
{
  auto kind = static_cast<epcstd::CommandKind>(getCommandKind());
  std::stringstream ss;
  ss << NAME << " {" <<
          "Command=" << epcstd::str(kind) <<
          ", Preamble=" << getPreamble().info() <<
          "}";
  return ss.str();
}

std::string SendReplyReq::info() const
{
  auto kind = static_cast<epcstd::ReplyKind>(getReplyKind());
  std::stringstream ss;
  ss << NAME << " {" <<
          "Reply=" << epcstd::str(kind) <<
          ", Preamble=" << getPreamble().info() <<
          "}";
  return ss.str();
}

std::string RecvBeginInd::info() const
{
  std::stringstream ss;
  ss << NAME << "{}";
  return ss.str();
}

std::string RecvCommandInd::info() const
{
  auto kind = static_cast<epcstd::CommandKind>(getCommandKind());
  std::stringstream ss;
  ss << NAME << " {" <<
          "Command=" << epcstd::str(kind) <<
          ", Preamble=" << getPreamble().info() <<
          "}";
  return ss.str();
}

std::string RecvReplyInd::info() const
{
  auto kind = static_cast<epcstd::ReplyKind>(getReplyKind());
  std::stringstream ss;
  ss << NAME << " {" <<
          "Reply=" << epcstd::str(kind) <<
          ", Preamble=" << getPreamble().info() <<
          "}";
  return ss.str();
}

std::string RecvErrorInd::info() const
{
  std::stringstream ss;
  ss << NAME << " {"
          << "Error=" << strRecvErrorType(static_cast<RecvErrorType>(getType()))
          << "}";
  return ss.str();
}

std::string ReaderAirFrame::info() const
{
  auto kind = static_cast<epcstd::CommandKind>(getCommandKind());
  std::stringstream ss;
  ss << NAME << " {" <<
          "Command=" << epcstd::str(kind) <<
          ", DeviceID=" << getDeviceID() <<
          ", PreambleDuration=" << getPreambleDuration() <<
          ", BodyDuration=" << getBodyDuration() <<
          ", ModulationLoss=" << getModulationLoss() <<
          ", Bandwidth=" << getBandwidth() <<
          ", BodyBitLength=" << getBodyBitLength() <<
          "}";
  return ss.str();
}

std::string TagAirFrame::info() const
{
  auto kind = static_cast<epcstd::ReplyKind>(getReplyKind());
  std::stringstream ss;
  ss << NAME << " {" <<
          "Reply=" << epcstd::str(kind) <<
          ", DeviceID=" << getDeviceID() <<
          ", PreambleDuration=" << getPreambleDuration() <<
          ", BodyDuration=" << getBodyDuration() <<
          ", ModulationLoss=" << getModulationLoss() <<
          ", Bandwidth=" << getBandwidth() <<
          ", BodyBitLength=" << getBodyBitLength() <<
          "}";
  return ss.str();
}

}
