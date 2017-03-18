#include <protocol/epcstd-command-encoder.h>

using namespace omnetpp;

namespace rfidsim {
namespace epcstd {

const char *getEncodedField(TagEncoding value, CommandType cmd)
{
  if (cmd == COMMAND_QUERY) {
    switch (value) {
      case FM_0: return "00";
      case MILLER_2: return "01";
      case MILLER_4: return "10";
      case MILLER_8: return "11";
      default: throw cRuntimeError("unrecognized TagEncoding = %d", value);
    }
  } else {
    throw cRuntimeError("TagEncoding is not used in %s", str(cmd));
  }
}

const char *getEncodedField(DivideRatio value, CommandType cmd)
{
  if (cmd == COMMAND_QUERY) {
    switch (value) {
      case DR_8: return "0";
      case DR_64_3: return "1";
      default: throw cRuntimeError("unrecognized DivideRatio = %d", value);
    }
  } else {
    throw cRuntimeError("DivideRatio is not used in %s", str(cmd));
  }
}

const char *getEncodedField(Sel value, CommandType cmd)
{
  if (cmd == COMMAND_QUERY) {
    switch (value) {
      case SEL_ALL: return "01";
      case SEL_NOT: return "10";
      case SEL_YES: return "11";
      default: throw cRuntimeError("unrecognized Sel = %d", value);
    }
  } else {
    throw cRuntimeError("Sel is not used in %s", str(cmd));
  }
}

const char *getEncodedField(Session value, CommandType cmd)
{
  if (cmd == COMMAND_QUERY || cmd == COMMAND_QUERY_REP) {
    switch (value) {
      case SESSION_0: return "00";
      case SESSION_1: return "01";
      case SESSION_2: return "10";
      case SESSION_3: return "11";
      default: throw cRuntimeError("unrecognized Session = %d", value);
    }
  } else {
    throw cRuntimeError("Session is not used in %s", str(cmd));
  }
}

const char *getEncodedField(InventoryFlag value, CommandType cmd)
{
  if (cmd == COMMAND_QUERY) {
    switch (value) {
      case FLAG_A: return "0";
      case FLAG_B: return "1";
      default: throw cRuntimeError("unrecognized InventoryFlag = %d", value);
    }
  } else {
    throw cRuntimeError("InventoryFlag is not used in %s", str(cmd));
  }
}

const char *getEncodedField(MemoryBank value, CommandType cmd)
{
  if (cmd == COMMAND_READ) {
    switch (value) {
      case BANK_EPC: return "01";
      case BANK_TID: return "10";
      case BANK_USER: return "11";
      default: throw cRuntimeError("unrecognized MemoryBank = %d", value);
    }
  } else {
    throw cRuntimeError("MemoryBank is not used in %s", str(cmd));
  }
}

const char *getEncodedCommandCode(CommandType cmd)
{
  switch (cmd) {
    case COMMAND_QUERY: return "1000";
    case COMMAND_QUERY_REP: return "00";
    case COMMAND_ACK: return "01";
    case COMMAND_REQ_RN: return "11000001";
    case COMMAND_READ: return "11000010";
    default: throw cRuntimeError("unrecognized CommandType = %d", cmd);
  }
}

unsigned encodeCommandCode(CommandType cmd, char *buf, unsigned offset,
                           unsigned buf_size)
{
  if (buf_size <= offset)
    throw cRuntimeError("encoding buffer overflow");

  unsigned n = buf_size - offset;
  int ret = snprintf(buf + offset, n, "%s", getEncodedCommandCode(cmd));

  if (ret < 0 || ret >= static_cast<int>(n))
    throw cRuntimeError("encoding buffer overflow");

  return static_cast<unsigned>(ret);
}

unsigned encodeValue(unsigned value, unsigned bitlen, char *buf,
                     unsigned offset, unsigned buf_size)
{
  if (offset + bitlen >= buf_size)
    throw cRuntimeError("encoding buffer overflow");

  for (unsigned i = 0; i < bitlen; ++i) {
    unsigned bit = value % 2;
    char s_bit = bit ? '1' : '0';
    value = value >> 1;
    buf[offset + bitlen - i] = s_bit;
  }
  buf[offset + bitlen] = '\0';
  return static_cast<unsigned>(bitlen);
}

unsigned encodeValue(bool value, char *buf, unsigned offset, unsigned buf_size)
{
  if (offset + 1 >= buf_size)
    throw cRuntimeError("encoding buffer overflow");
  buf[offset] = value ? '1' : '0';
  buf[offset + 1] = '\0';
  return 1;
}

unsigned encodeValueEBV(unsigned value, char *buf, unsigned offset,
                        unsigned buf_size)
{
  //TODO: now we encode it as normal unsigned, implement EBV correctly!
  if (value < 128)
    return encodeValue(value, 8, buf, offset, buf_size);
  else if (value < 16384)
    return encodeValue(value, 16, buf, offset, buf_size);
  else
    return encodeValue(value, 24, buf, offset, buf_size);
}

unsigned getCRC16(char *buf, unsigned offset, unsigned buf_size)
{
  //TODO: implement CRC16 according to standard
  return 0xFFFF;
}

unsigned getCRC5(char *buf, unsigned offset, unsigned buf_size)
{
  //TODO: implement CRC5 according to standard
  return 0x1F;
}


//
//===========================================================================
// CommandEncoder
//===========================================================================
//
const unsigned CommandEncoder::DEFAULT_BUFFER_SIZE = 1024;

CommandEncoder::CommandEncoder(unsigned buffer_size)
: offset(0), buf_size(buffer_size)
{
  buf = new char[buffer_size];
  buf[0] = '\0';
}

CommandEncoder::~CommandEncoder()
{
  delete[] buf;
}

void CommandEncoder::encode(Command *cmd)
{
  if (!cmd) throw cRuntimeError("cmd=<%p>", cmd);

  buf[0] = '\0';
  offset = 0;

  auto kind = cmd->getKind();
  if (kind == KIND_COMMAND_QUERY)
    encodeQuery(check_and_cast<Query*>(cmd));
  else if (kind == KIND_COMMAND_QUERY_REP)
    encodeQueryRep(check_and_cast<QueryRep*>(cmd));
  else if (kind == KIND_COMMAND_ACK)
    encodeACK(check_and_cast<Ack*>(cmd));
  else if (kind == KIND_COMMAND_REQ_RN)
    encodeReqRN(check_and_cast<ReqRN*>(cmd));
  else if (kind == KIND_COMMAND_READ)
    encodeRead(check_and_cast<Read*>(cmd));
  else
    throw cRuntimeError("unsupported command kind = %d", kind);
}

const char *CommandEncoder::getEncodedString() const
{
  return buf;
}

unsigned CommandEncoder::getBitLength() const
{
  return offset;
}

void CommandEncoder::encodeQuery(Query *cmd)
{
  offset += encodeCommandCode(COMMAND_QUERY, buf, offset, buf_size);
  offset += encodeField(static_cast<DivideRatio>(cmd->getDR()), COMMAND_QUERY,
                       buf, offset, buf_size);
  offset += encodeField(static_cast<TagEncoding>(cmd->getM()), COMMAND_QUERY,
                        buf, offset, buf_size);
  offset += encodeValue(cmd->getTRext(), buf, offset, buf_size);
  offset += encodeField(static_cast<Sel>(cmd->getSel()), COMMAND_QUERY, buf,
                        offset, buf_size);
  offset += encodeField(static_cast<Session>(cmd->getSession()), COMMAND_QUERY,
                        buf, offset, buf_size);
  offset += encodeField(static_cast<InventoryFlag>(cmd->getTarget()),
                        COMMAND_QUERY, buf, offset, buf_size);
  offset += encodeValue(cmd->getQ(), 4, buf, offset, buf_size);
  unsigned crc = getCRC5(buf, 0, offset);
  cmd->setCRC5(crc);
  offset += encodeValue(crc, 5, buf, offset, buf_size);
}

void CommandEncoder::encodeQueryRep(QueryRep *cmd)
{
  offset += encodeCommandCode(COMMAND_QUERY_REP, buf, offset, buf_size);
  offset += encodeField(static_cast<Session>(cmd->getSession()),
                        COMMAND_QUERY_REP, buf, offset, buf_size);
}

void CommandEncoder::encodeACK(Ack *cmd)
{
  offset += encodeCommandCode(COMMAND_ACK, buf, offset, buf_size);
  offset += encodeValue(cmd->getRN(), 16, buf, offset, buf_size);
}

void CommandEncoder::encodeReqRN(ReqRN *cmd)
{
  offset += encodeCommandCode(COMMAND_REQ_RN, buf, offset, buf_size);
  offset += encodeValue(cmd->getRN(), 16, buf, offset, buf_size);
  unsigned crc = getCRC16(buf, 0, offset);
  cmd->setCRC16(crc);
  offset += encodeValue(crc, 16, buf, offset, buf_size);
}

void CommandEncoder::encodeRead(Read *cmd)
{
  offset += encodeCommandCode(COMMAND_READ, buf, offset, buf_size);
  offset += encodeField(static_cast<MemoryBank>(cmd->getBank()), COMMAND_READ,
                        buf, offset, buf_size);
  offset += encodeValueEBV(cmd->getWordPtr(), buf, offset, buf_size);
  offset += encodeValue(cmd->getWordCount(), 8, buf, offset, buf_size);
  offset += encodeValue(cmd->getRN(), 16, buf, offset, buf_size);
  unsigned crc = getCRC16(buf, 0, offset);
  cmd->setCRC16(crc);
  offset += encodeValue(crc, 16, buf, offset, buf_size);
}

}}
