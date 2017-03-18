#ifndef RFIDSIMPP_LOGIC_READER_LOGIC_LAYER_SIGNALS_H_
#define RFIDSIMPP_LOGIC_READER_LOGIC_LAYER_SIGNALS_H_

#include <omnetpp.h>
#include <common/optional.hpp>

namespace rfidsim {

class TagReadData : public omnetpp::cObject, omnetpp::noncopyable {
 public:
  static const char *NAME;

  nonstd::optional<std::vector<uint8_t>> epc;
  nonstd::optional<std::vector<uint8_t>> tid;
  unsigned long round_index;
  unsigned slot_index;
  int reader_id;
  int tag_id;

  virtual std::string info() const;
};

class NewRound : public omnetpp::cObject, omnetpp::noncopyable {
 public:
  static const char *NAME;

  int reader_id;
  unsigned long round_index;
  unsigned slots_number;

  virtual std::string info() const;
};

class NewSlot : public omnetpp::cObject, omnetpp::noncopyable {
 public:
  static const char *NAME;

  int reader_id;
  unsigned long round_index;
  unsigned slot_index;

  virtual std::string info() const;
};

}

#endif
