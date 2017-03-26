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
// Commands encoding
//===========================================================================
//
unsigned encode(Command *cmd, char *buf, unsigned size)
{
  if (!cmd) throw cRuntimeError("cmd=<%p>", cmd);
  if (!buf) throw cRuntimeError("buf=<%p>", buf);
  if (size == 0) throw cRuntimeError("buffer size=%u passed", size);

  buf[0] = '\0';

  auto kind = cmd->getKind();
  if (kind == KIND_COMMAND_QUERY)
    return encodeQuery(check_and_cast<Query*>(cmd), buf, size);
  else if (kind == KIND_COMMAND_QUERY_REP)
    return encodeQueryRep(check_and_cast<QueryRep*>(cmd), buf, size);
  else if (kind == KIND_COMMAND_ACK)
    return encodeACK(check_and_cast<Ack*>(cmd), buf, size);
  else if (kind == KIND_COMMAND_REQ_RN)
    return encodeReqRN(check_and_cast<ReqRN*>(cmd), buf, size);
  else if (kind == KIND_COMMAND_READ)
    return encodeRead(check_and_cast<Read*>(cmd), buf, size);
  else
    throw cRuntimeError("unsupported command kind = %d", kind);
}

unsigned encodeQuery(Query *cmd, char *buf, unsigned size)
{
  unsigned offset = 0;
  offset += encodeCommandCode(COMMAND_QUERY, buf, offset, size);
  offset += encodeField(static_cast<DivideRatio>(cmd->getDR()), COMMAND_QUERY,
                       buf, offset, size);
  offset += encodeField(static_cast<TagEncoding>(cmd->getM()), COMMAND_QUERY,
                        buf, offset, size);
  offset += encodeValue(cmd->getTRext(), buf, offset, size);
  offset += encodeField(static_cast<Sel>(cmd->getSel()), COMMAND_QUERY, buf,
                        offset, size);
  offset += encodeField(static_cast<Session>(cmd->getSession()), COMMAND_QUERY,
                        buf, offset, size);
  offset += encodeField(static_cast<InventoryFlag>(cmd->getTarget()),
                        COMMAND_QUERY, buf, offset, size);
  offset += encodeValue(cmd->getQ(), 4, buf, offset, size);
  unsigned crc = getCRC5(buf, 0, offset);
  cmd->setCRC5(crc);
  offset += encodeValue(crc, 5, buf, offset, size);
  return offset;
}

unsigned encodeQueryRep(QueryRep *cmd, char *buf, unsigned size)
{
  unsigned offset = 0;
  offset += encodeCommandCode(COMMAND_QUERY_REP, buf, offset, size);
  offset += encodeField(static_cast<Session>(cmd->getSession()),
                        COMMAND_QUERY_REP, buf, offset, size);
  return offset;
}

unsigned encodeACK(Ack *cmd, char *buf, unsigned size)
{
  unsigned offset = 0;
  offset += encodeCommandCode(COMMAND_ACK, buf, offset, size);
  offset += encodeValue(cmd->getRN(), 16, buf, offset, size);
  return offset;
}

unsigned encodeReqRN(ReqRN *cmd, char *buf, unsigned size)
{
  unsigned offset = 0;
  offset += encodeCommandCode(COMMAND_REQ_RN, buf, offset, size);
  offset += encodeValue(cmd->getRN(), 16, buf, offset, size);
  unsigned crc = getCRC16(buf, 0, offset);
  cmd->setCRC16(crc);
  offset += encodeValue(crc, 16, buf, offset, size);
  return offset;
}

unsigned encodeRead(Read *cmd, char *buf, unsigned size)
{
  unsigned offset = 0;
  offset += encodeCommandCode(COMMAND_READ, buf, offset, size);
  offset += encodeField(static_cast<MemoryBank>(cmd->getBank()), COMMAND_READ,
                        buf, offset, size);
  offset += encodeValueEBV(cmd->getWordPtr(), buf, offset, size);
  offset += encodeValue(cmd->getWordCount(), 8, buf, offset, size);
  offset += encodeValue(cmd->getRN(), 16, buf, offset, size);
  unsigned crc = getCRC16(buf, 0, offset);
  cmd->setCRC16(crc);
  offset += encodeValue(crc, 16, buf, offset, size);
  return offset;
}

unsigned countBits(const char *buf, unsigned *n_zeros, unsigned *n_ones)
{
  unsigned n0s = 0;
  unsigned n1s = 0;
  unsigned bitlen = strlen(buf);
  for (unsigned i = 0; i < bitlen; ++i)
  {
    char b = buf[i];
    if (b == '0')
      n0s += 1;
    else if (b == '1')
      n1s += 1;
    else
      throw cRuntimeError("bit string '%s' contains ill symbol '%c'", buf, b);
  }
  if (n_zeros) *n_zeros = n0s;
  if (n_ones) *n_ones = n1s;
  return bitlen;
}

}}
