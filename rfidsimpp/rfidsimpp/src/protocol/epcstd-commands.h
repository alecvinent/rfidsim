#ifndef RFIDSIMPP_PROTOCOL_EPCSTD_COMMANDS_H_
#define RFIDSIMPP_PROTOCOL_EPCSTD_COMMANDS_H_

#include <omnetpp.h>
#include <protocol/epcstd-commands_m.h>

namespace rfidsim {
namespace epcstd {

class ReaderPreamble : public omnetpp::cOwnedObject {
public:
  static const omnetpp::simtime_t DEFAULT_DELIM;

  enum Type {
    PREAMBLE, SYNC
  };

  static ReaderPreamble createPreamble(omnetpp::simtime_t tari,
      omnetpp::simtime_t rtcal, omnetpp::simtime_t trcal,
      omnetpp::simtime_t delim = DEFAULT_DELIM);

  static ReaderPreamble createSync(omnetpp::simtime_t tari,
      omnetpp::simtime_t rtcal, omnetpp::simtime_t delim = DEFAULT_DELIM);

  ReaderPreamble() {}

  ReaderPreamble(const ReaderPreamble& other)
    : type_(other.type_), tari_(other.tari_), rtcal_(other.rtcal_),
      trcal_(other.trcal_), delim_(other.delim_) {}

  virtual ~ReaderPreamble() {}

  ReaderPreamble& operator=(const ReaderPreamble& rside) {
    type_ = rside.getType();
    tari_ = rside.getTari();
    rtcal_ = rside.getRTcal();
    trcal_ = rside.getTRcal();
    delim_ = rside.getDelim();
    return *this;
  }

  virtual ReaderPreamble *dup() const {
    return new ReaderPreamble(*this);
  }

  Type getType() const { return type_; }
  omnetpp::simtime_t getTari() const { return tari_; }
  omnetpp::simtime_t getRTcal() const { return rtcal_; }
  omnetpp::simtime_t getTRcal() const { return trcal_; }
  omnetpp::simtime_t getDelim() const { return delim_; }

  void setType(Type type) { type_ = type; }
  void setTari(omnetpp::simtime_t tari) { tari_ = tari; }
  void setRTcal(omnetpp::simtime_t rtcal) { rtcal_ = rtcal; }
  void setTRcal(omnetpp::simtime_t trcal) { trcal_ = trcal; }
  void setDelim(omnetpp::simtime_t delim) { delim_ = delim; }

  void setRTcalAsTariMul(double k) { rtcal_ = k * tari_; }
  void setTRcalAsRTcalMul(double k) { trcal_ = k * rtcal_; }

  omnetpp::simtime_t getData0() const { return tari_; }
  omnetpp::simtime_t getData1() const { return rtcal_ - tari_; }

  omnetpp::simtime_t getDuration() const {
    return delim_ + tari_ + rtcal_ +
        (type_ == PREAMBLE ? trcal_ : omnetpp::SimTime::ZERO);
  }

 private:
  Type type_ = SYNC;
  omnetpp::simtime_t tari_ = omnetpp::SimTime::ZERO;
  omnetpp::simtime_t rtcal_ = omnetpp::SimTime::ZERO;
  omnetpp::simtime_t trcal_ = omnetpp::SimTime::ZERO;
  omnetpp::simtime_t delim_ = DEFAULT_DELIM;
};


class Query : public Query_Base {
 public:
  static const char *NAME;

  Query() : Query_Base(NAME, KIND_COMMAND_QUERY) {}
  Query(const Query& other) : Query_Base(other) {}
  virtual ~Query() {}

  virtual Query *dup() const { return new Query(*this); }

  Query& operator=(const Query& rside) {
    Query_Base::operator=(rside);
    return *this;
  }

  virtual std::string info() const;
};


class QueryRep : public QueryRep_Base {
 public:
  static const char *NAME;

  QueryRep() : QueryRep_Base(NAME, KIND_COMMAND_QUERY_REP) {}
  QueryRep(const QueryRep& other) : QueryRep_Base(other) {}
  virtual ~QueryRep() {}

  virtual QueryRep *dup() const { return new QueryRep(*this); }

  QueryRep& operator=(const QueryRep& rside) {
    QueryRep_Base::operator=(rside);
    return *this;
  }

  virtual std::string info() const;
};


class Ack : public Ack_Base {
 public:
  static const char *NAME;

  Ack() : Ack_Base(NAME, KIND_COMMAND_ACK) {}
  Ack(const Ack& other) : Ack_Base(other) {}
  virtual ~Ack() {}

  virtual Ack *dup() const { return new Ack(*this); }

  Ack& operator=(const Ack& rside) {
    Ack_Base::operator=(rside);
    return *this;
  }

  virtual std::string info() const;
};


class ReqRN : public ReqRN_Base {
 public:
  static const char *NAME;

  ReqRN() : ReqRN_Base(NAME, KIND_COMMAND_REQ_RN) {}
  ReqRN(const ReqRN& other) : ReqRN_Base(other) {}
  virtual ~ReqRN() {}

  virtual ReqRN *dup() const { return new ReqRN(*this); }

  ReqRN& operator=(const ReqRN& rside) {
    ReqRN_Base::operator=(rside);
    return *this;
  }

  virtual std::string info() const;
};


class Read : public Read_Base {
 public:
  static const char *NAME;

  Read() : Read_Base(NAME, KIND_COMMAND_READ) {}
  Read(const Read& other) : Read_Base(other) {}
  virtual ~Read() {}

  virtual Read *dup() const { return new Read(*this); }

  Read& operator=(const Read& rside) {
    Read_Base::operator=(rside);
    return *this;
  }

  virtual std::string info() const;
};

}}

#endif
