#ifndef RFIDSIMPP_LOGIC_READER_TAGOPS_H_
#define RFIDSIMPP_LOGIC_READER_TAGOPS_H_

#include <omnetpp.h>
#include <logic/reader-tagops_m.h>

namespace rfidsim {
namespace tagop {

const char *str(FailureCode code);

class Acknowledge : public Acknowledge_Base {
 public:
  static const char *NAME;

  Acknowledge() : Acknowledge_Base(NAME, KIND_ACK) {}
  Acknowledge(const Acknowledge& other) : Acknowledge_Base(other) {}
  virtual ~Acknowledge() {}

  virtual Acknowledge *dup() const { return new Acknowledge(*this); }

  Acknowledge& operator=(const Acknowledge& rside) {
    Acknowledge_Base::operator =(rside);
    return *this;
  }

  virtual std::string info() const;
};

class ReadBank : public ReadBank_Base {
 public:
  static const char *NAME;

  ReadBank() : ReadBank_Base(NAME, KIND_READ_BANK) {}
  ReadBank(const ReadBank& other) : ReadBank_Base(other) {}
  virtual ~ReadBank() {}

  virtual ReadBank *dup() const { return new ReadBank(*this); }

  ReadBank& operator=(const ReadBank& rside) {
    ReadBank_Base::operator =(rside);
    return *this;
  }

  virtual std::string info() const;
};

class Success : public Success_Base {
 public:
  static const char *NAME;

  Success() : Success_Base(NAME, KIND_SUCCESS) {}
  Success(const Success& other) : Success_Base(other) {}
  virtual ~Success() {}

  virtual Success *dup() const { return new Success(*this); }

  Success& operator=(const Success& rside) {
    Success_Base::operator=(rside);
    return *this;
  }

  virtual std::string info() const;
};

class Failure : public Failure_Base {
 public:
  static const char *NAME;

  Failure() : Failure_Base(NAME, KIND_FAILURE) {}
  Failure(const Failure& other) : Failure_Base(other) {}
  virtual ~Failure() {}

  virtual Failure *dup() const { return new Failure(*this); }

  Failure& operator=(const Failure& rside) {
    Failure_Base::operator=(rside);
    return *this;
  }

  virtual std::string info() const;
};

class TagInfo : public TagInfo_Base {
 public:
  static const char *NAME;

  TagInfo() : TagInfo_Base(NAME, KIND_TAGINFO) {}
  TagInfo(const TagInfo& other) : TagInfo_Base(other) {}
  virtual ~TagInfo() {}

  virtual TagInfo *dup() const { return new TagInfo(*this); }

  TagInfo& operator=(const TagInfo& rside) {
    TagInfo_Base::operator=(rside);
    return *this;
  }

  virtual std::string info() const;

  virtual void setEPCByteLength(unsigned int size) { epc.resize(size, 0); }
  virtual unsigned int getEPCByteLength() const { return epc.size(); }
  virtual uint8_t getEPCByte(unsigned int k) const { return epc[k]; }
  virtual void setEPCByte(unsigned int k, uint8_t b) { epc[k] = b; }
  void setEPC(const std::vector<uint8_t>& epc) { this->epc = epc; }
  const std::vector<uint8_t>& getEPC() const { return epc; }

 private:
  std::vector<uint8_t> epc;
};

class Data : public Data_Base {
 public:
  static const char *NAME;

  Data() : Data_Base(NAME, KIND_TAGINFO) {}
  Data(const Data& other) : Data_Base(other) {}
  virtual ~Data() {}

  virtual Data *dup() const { return new Data(*this); }

  Data& operator=(const Data& rside) {
    Data_Base::operator =(rside);
    return *this;
  }

  virtual std::string info() const;

  virtual void setDataByteLength(unsigned int size) { data.resize(size, 0); }
  virtual unsigned int getDataByteLength() const { return data.size(); }
  virtual uint8_t getDataByte(unsigned int k) const { return data[k]; }
  virtual void setDataByte(unsigned int k, uint8_t b) { data[k] = b; }
  const std::vector<uint8_t>& getData() const { return data; }

 private:
  std::vector<uint8_t> data;
};

}}

#endif
