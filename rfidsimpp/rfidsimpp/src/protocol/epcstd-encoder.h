#ifndef RFIDSIMPP_PROTOCOL_EPCSTD_COMMAND_ENCODER_H_
#define RFIDSIMPP_PROTOCOL_EPCSTD_COMMAND_ENCODER_H_

#include <omnetpp.h>
#include <protocol/epcstd-base.h>
#include <protocol/epcstd-commands.h>
#include <protocol/epcstd-responses.h>

namespace rfidsim {
namespace epcstd {

const char *getEncodedField(TagEncoding value, CommandType cmd);
const char *getEncodedField(DivideRatio value, CommandType cmd);
const char *getEncodedField(Sel value, CommandType cmd);
const char *getEncodedField(Session value, CommandType cmd);
const char *getEncodedField(InventoryFlag value, CommandType cmd);
const char *getEncodedField(MemoryBank value, CommandType cmd);

const char *getEncodedCommandCode(CommandType cmd);

template<typename T>
unsigned encodeField(T value, CommandType cmd, char *buf, unsigned offset,
                     unsigned buf_size)
{
  if (buf_size <= offset)
    throw omnetpp::cRuntimeError("encoding buffer overflow");

  unsigned n = buf_size - offset;
  int ret = snprintf(buf + offset, n, "%s", getEncodedField(value, cmd));

  if (ret < 0 || ret >= static_cast<int>(n))
    throw omnetpp::cRuntimeError("encoding buffer overflow");

  return static_cast<unsigned>(ret);
}

unsigned getEncodeCommandCode(CommandType cmd,
                              char *buf, unsigned offset, unsigned buf_size);

unsigned encodeValue(unsigned value, unsigned bitlen, char *buf,
                     unsigned offset, unsigned buf_size);

unsigned encodeValue(bool value, char *buf, unsigned offset, unsigned buf_size);

unsigned encodeValueEBV(unsigned value, char *buf, unsigned offset,
                        unsigned buf_size);

unsigned getCRC16(char *buf, unsigned offset, unsigned buf_size);
unsigned getCRC5(char *buf, unsigned offset, unsigned buf_size);

unsigned encode(Command *cmd, char *buf, unsigned size);
unsigned encodeQuery(Query *cmd, char *buf, unsigned size);
unsigned encodeQueryRep(QueryRep *cmd, char *buf, unsigned size);
unsigned encodeACK(Ack *cmd, char *buf, unsigned size);
unsigned encodeReqRN(ReqRN *cmd, char *buf, unsigned size);
unsigned encodeRead(Read *cmd, char *buf, unsigned size);

unsigned countBits(const char *buf, unsigned *n_zeros, unsigned *n_ones);

unsigned getBitLen(Reply *reply);
unsigned getQueryReplyBitLen(QueryReply *reply);
unsigned getAckReplyBitLen(AckReply *reply);
unsigned getReqRNReplyBitLen(ReqRNReply *reply);
unsigned getReadReplyBitLen(ReadReply *reply);

}}

#endif
