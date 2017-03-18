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

const char *SendCommandReq::NAME = "SendCommandReq";
const char *SendReplyReq::NAME = "SendReplyReq";
const char *RecvBeginInd::NAME = "RecvBeginInd";
const char *RecvCommandInd::NAME = "RecvCommandInd";
const char *RecvReplyInd::NAME = "RecvReplyInd";
const char *RecvErrorInd::NAME = "RecvErrorInd";

std::string SendCommandReq::info() const
{
  auto cmd = getEncapsulatedPacket();
  std::stringstream ss;
  ss << NAME << " {" << "preamble:" << getPreamble().info() << ", command:" <<
          (cmd ? cmd->info() : "(none)") << "}";
  return ss.str();
}

std::string SendReplyReq::info() const
{
  auto reply = getEncapsulatedPacket();
  std::stringstream ss;
  ss << NAME << " {" << "preamble:" << getPreamble().info() << ", command:" <<
          (reply ? reply->info() : "(none)") << "}";
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
  auto cmd = getEncapsulatedPacket();
  std::stringstream ss;
  ss << NAME << " {" << "preamble:" << getPreamble().info() << ", command:" <<
          (cmd ? cmd->info() : "(none)") << "}";
  return ss.str();
}

std::string RecvReplyInd::info() const
{
  auto reply = getEncapsulatedPacket();
  std::stringstream ss;
  ss << NAME << " {" << "preamble:" << getPreamble().info() << ", command:" <<
          (reply ? reply->info() : "(none)") << "}";
  return ss.str();
}

std::string RecvErrorInd::info() const
{
  std::stringstream ss;
  ss << NAME << " {"
          << "error=" << strRecvErrorType(static_cast<RecvErrorType>(getType()))
          << "}";
  return ss.str();
}

}
