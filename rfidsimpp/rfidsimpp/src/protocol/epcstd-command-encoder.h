#ifndef RFIDSIMPP_PROTOCOL_EPCSTD_COMMAND_ENCODER_H_
#define RFIDSIMPP_PROTOCOL_EPCSTD_COMMAND_ENCODER_H_

#include <omnetpp.h>
#include <protocol/epcstd-base.h>
#include <protocol/epcstd-commands.h>

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


class CommandEncoder {
 public:
  static const unsigned DEFAULT_BUFFER_SIZE;

  CommandEncoder(unsigned buffer_size = DEFAULT_BUFFER_SIZE);
  virtual ~CommandEncoder();

  void encode(Command *cmd);
  const char *getEncodedString() const;
  unsigned getBitLength() const;
 private:
  unsigned offset = 0;
  unsigned buf_size = 0;
  char *buf = nullptr;

  void encodeQuery(Query *cmd);
  void encodeQueryRep(QueryRep *cmd);
  void encodeACK(Ack *cmd);
  void encodeReqRN(ReqRN *cmd);
  void encodeRead(Read *cmd);
};

}}

#endif
