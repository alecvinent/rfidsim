#ifndef RFIDSIMPP_LOGIC_CONTROL_READER_LOGIC_CONTROLLER_H_
#define RFIDSIMPP_LOGIC_CONTROL_READER_LOGIC_CONTROLLER_H_

#include <omnetpp.h>
#include <logic/logic-layer-base.h>

namespace rfidsim {

class ReaderLogicLayer;

namespace reader {
namespace logic {

class Controller : public omnetpp::cObject {
 public:
  Controller() {}
  Controller(const Controller& other) {}
  virtual ~Controller() {}

  ReaderLogicLayer *getLogicLayerModule() const { return module; }

  virtual void setUp(ReaderLogicLayer *module, omnetpp::cXMLElement *xml) {
    this->module = module;
    init(xml);
  }

  virtual void incCollisionsNum() = 0;
  virtual void incChannelErrorsNum() = 0;
  virtual void incNoReplyErrorsNum() = 0;
  virtual void incEmptySlotsNum() = 0;
  virtual void incTagsInventoriedNum() = 0;
  virtual void incTagsProcessedNum() = 0;
  virtual void finishRound() = 0;

  virtual void getParams(const RoundDescriptor& curr_descr,
                         RoundDescriptor *next_descr) = 0;

 protected:
  virtual void init(omnetpp::cXMLElement *xml) = 0;

 private:
  ReaderLogicLayer *module = nullptr;
};


class ConstantController : public Controller {
 public:
  ConstantController() {}
  ConstantController(const ConstantController& other) : Controller(other) {}
  virtual ~ConstantController() {}

  ConstantController& operator=(const ConstantController& risde) {
    return *this;
  }

  virtual ConstantController *dup() const {
    return new ConstantController(*this);
  }

  virtual void incCollisionsNum() {}
  virtual void incChannelErrorsNum() {}
  virtual void incNoReplyErrorsNum() {}
  virtual void incEmptySlotsNum() {}
  virtual void incTagsInventoriedNum() {}
  virtual void incTagsProcessedNum() {}
  virtual void finishRound() {}

  virtual void getParams(const RoundDescriptor& curr_descr,
                         RoundDescriptor *next_descr) {
    if (next_descr) *next_descr = curr_descr;
  }

  virtual std::string info() const { return "ConstantController"; }

 protected:
  virtual void init(omnetpp::cXMLElement *xml) {}
};


}}}

#endif
