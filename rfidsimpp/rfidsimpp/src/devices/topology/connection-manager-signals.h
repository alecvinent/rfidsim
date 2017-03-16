#ifndef RFIDSIMPP_DEVICES_TOPOLOGY_CONNECTION_MANAGER_SIGNALS_H_
#define RFIDSIMPP_DEVICES_TOPOLOGY_CONNECTION_MANAGER_SIGNALS_H_

#include <omnetpp.h>

namespace rfidsim {

class ConnectionCreated : public omnetpp::cObject, omnetpp::noncopyable {
public:
  static const char *NAME;

  int reader_id = -1;
  int tag_id = -1;

  ConnectionCreated() {}

  ConnectionCreated(int reader_id, int tag_id)
    : reader_id(reader_id), tag_id(tag_id) {}

  virtual ~ConnectionCreated() {}

  virtual std::string info() const;
};

class ConnectionLost : public omnetpp::cObject, omnetpp::noncopyable {
public:
  static const char *NAME;

  int reader_id = -1;
  int tag_id = -1;

  ConnectionLost() {}

  ConnectionLost(int reader_id, int tag_id)
    : reader_id(reader_id), tag_id(tag_id) {}

  virtual ~ConnectionLost() {}

  virtual std::string info() const;
};

}

#endif
