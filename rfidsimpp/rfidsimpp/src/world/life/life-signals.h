#ifndef RFIDSIMPP_WORLD_LIFE_LIFE_SIGNALS_H_
#define RFIDSIMPP_WORLD_LIFE_LIFE_SIGNALS_H_

#include <omnetpp.h>

namespace rfidsim {

class DestroyThing : public omnetpp::cObject, omnetpp::noncopyable {
 public:
  static const char *NAME;
  int thing_id = -1;

  DestroyThing() {}
  DestroyThing(int thing_id) : thing_id(thing_id) {}
  virtual ~DestroyThing() {}

  virtual std::string info() const;
};

}

#endif
