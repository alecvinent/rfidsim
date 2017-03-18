#ifndef RFIDSIMPP_PROTOCOL_EPCSTD_RESPONSES_H_
#define RFIDSIMPP_PROTOCOL_EPCSTD_RESPONSES_H_

#include <omnetpp.h>
#include <protocol/epcstd-responses_m.h>
#include <cstdint>

namespace rfidsim {
namespace epcstd {

const char *str(ReplyKind kind);


class TagPreamble : public omnetpp::cOwnedObject {
 public:
  TagPreamble() {}

  TagPreamble(TagEncoding m, DivideRatio dr, bool trext,
      omnetpp::simtime_t trcal)
    : m_(m), dr_(dr), trext_(trext), trcal_(trcal) {}

  TagPreamble(const TagPreamble& other)
    : m_(other.m_), dr_(other.dr_), trext_(other.trext_), trcal_(other.trcal_)
  {}

  virtual ~TagPreamble() {}

  TagEncoding getTagEncoding() const { return m_; }
  DivideRatio getDivideRatio() const { return dr_; }
  bool getTRext() const { return trext_; }
  omnetpp::simtime_t getTRcal() const { return trcal_; }

  void setTagEncoding(TagEncoding m) { m_ = m; }
  void setDivideRatio(DivideRatio dr) { dr_ = dr; }
  void setTRext(bool trext) { trext_ = trext; }
  void setTRcal(omnetpp::simtime_t trcal) { trcal_ = trcal; }

  int getBitLength() const;

  omnetpp::simtime_t getDuration() const {
    return getBitLength() * getTagBitRate(trcal_, dr_, m_);
  }

  virtual std::string info() const;

 private:
  TagEncoding m_;
  DivideRatio dr_;
  bool trext_;
  omnetpp::simtime_t trcal_;
};


class QueryReply : public QueryReply_Base {
 public:
  static const char *NAME;

  QueryReply() : QueryReply_Base(NAME) {}
  QueryReply(const QueryReply& other) : QueryReply_Base(other) {}
  virtual ~QueryReply() {}

  virtual QueryReply *dup() const { return new QueryReply(*this); }

  QueryReply& operator=(const QueryReply& other) {
    QueryReply_Base::operator=(other);
    return *this;
  }

  virtual std::string info() const;
};

class AckReply : public AckReply_Base {
 public:
  static const char *NAME;

  AckReply() : AckReply_Base(NAME) {}
  AckReply(const AckReply& other) : AckReply_Base(other) {}
  virtual ~AckReply() {}

  virtual AckReply *dup() const { return new AckReply(*this); }

  AckReply& operator=(const AckReply& other) {
    AckReply_Base::operator=(other);
    return *this;
  }

  virtual void setEPCByteLength(unsigned int size) { epc.resize(size, 0x00); }
  virtual unsigned int getEPCByteLength() const { return epc.size(); }
  virtual uint8_t getEPCByte(unsigned int k) const { return epc[k]; }
  virtual void setEPCByte(unsigned int k, uint8_t b) { epc[k] = b; }
  const std::vector<uint8_t>& getEPC() const { return epc; }

  virtual std::string info() const;
 private:
  std::vector<uint8_t> epc;
};

class ReqRNReply : public ReqRNReply_Base {
 public:
  static const char *NAME;

  ReqRNReply() : ReqRNReply_Base(NAME) {}
  ReqRNReply(const ReqRNReply& other) : ReqRNReply_Base(other) {}
  virtual ~ReqRNReply() {}

  virtual ReqRNReply *dup() const { return new ReqRNReply(*this); }

  ReqRNReply& operator=(const ReqRNReply& other) {
    ReqRNReply_Base::operator=(other);
    return *this;
  }

  virtual std::string info() const;
};

class ReadReply : public ReadReply_Base {
 public:
  static const char *NAME;

  ReadReply() : ReadReply_Base(NAME) {}
  ReadReply(const ReadReply& other) : ReadReply_Base(other) {}
  virtual ~ReadReply() {}

  virtual ReadReply *dup() const { return new ReadReply(*this); }

  ReadReply& operator=(const ReadReply& other) {
    ReadReply_Base::operator=(other);
    return *this;
  }

  virtual void setMemByteLength(unsigned int size) { memory.resize(size, 0); }
  virtual unsigned int getMemByteLength() const { return memory.size(); }
  virtual uint8_t getMemByte(unsigned int k) const { return memory[k]; }
  virtual void setMemByte(unsigned int k, uint8_t b) { memory[k] = b; }
  const std::vector<uint8_t>& getMem() const { return memory; }

  virtual std::string info() const;

 private:
  std::vector<uint8_t> memory;
};

}}

#endif
