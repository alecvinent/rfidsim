#ifndef RFIDSIMPP_LOGIC_READER_ROUND_MANAGER_H_
#define RFIDSIMPP_LOGIC_READER_ROUND_MANAGER_H_

#include <omnetpp.h>
#include <functional>
#include <protocol/epcstd-base.h>
#include <protocol/epcstd-commands.h>
#include <protocol/epcstd-responses.h>
#include <phy/phy-layer-messages.h>
#include <logic/logic-layer-base.h>
#include <logic/reader-logic-sap.h>

namespace rfidsim {

class ReaderLogicLayer;

class ReaderRoundManager : public omnetpp::cObject, omnetpp::noncopyable {
 public:
  enum State {
    OFF, READY, WAIT_HANDLE, WAIT_RESULT
  };

  enum Operation {
    NONE, QUERY_TAGS, ACKNOWLEDGE, READ_BANK
  };

  typedef std::function<void()> EmptyCb;
  typedef std::function<void(LogicError)> ErrorCb;
  typedef std::function<void(const std::vector<uint8_t>&)> DataCb;
  typedef std::function<void(long, unsigned,
                             const std::vector<uint8_t>&)> AckCb;

  static const char *str(State state);
  static const char *str(Operation operation);

  ReaderRoundManager(ReaderLogicLayer *logic_module, AckCb ack_cb,
                     DataCb data_cb, EmptyCb success_cb, ErrorCb error_cb);

  virtual ~ReaderRoundManager();

  void start(const RoundDescriptor& descr);
  void stop();
  void nextSlot();
  void acknowledge();
  void readBank(epcstd::MemoryBank bank, unsigned word_ptr, unsigned word_cnt);

  State getState() const { return state; }

  void setLogicModule(ReaderLogicLayer *logic_module) {
    this->logic_module = logic_module;
  }

  ReaderLogicLayer *getLogicModule() const {
    return logic_module;
  }

  void setSuccessCallback(EmptyCb callback) {
    success_cb = callback;
  }

  void setErrorCallback(ErrorCb callback) {
    error_cb = callback;
  }

  void setDataCallback(DataCb callback) {
    data_cb = callback;
  }

  void setTagReadCallback(AckCb callback) {
    ack_cb = callback;
  }

  unsigned getHandle() const {
    return handle;
  }

  void handlePhyDataConf(PhyDataConf *msg);

 protected:
  void processNoReplySuccess();
  void processQueryReply(epcstd::QueryReply *msg);
  void processAckReply(epcstd::AckReply *msg);
  void processReqRNReply(epcstd::ReqRNReply *msg);
  void processReadReply(epcstd::ReadReply *msg);
  void processPhyError(PhyDataConfStatus status);

  void setState(State state);
  void setOperation(Operation operation);
  void sendCmd(epcstd::Command *cmd);

 private:
  ReaderLogicLayer *logic_module = nullptr;
  RoundDescriptor round_descriptor;
  unsigned int handle = 0;
  State state = OFF;
  Operation operation = NONE;

  epcstd::MemoryBank bank;
  unsigned word_ptr = 0;
  unsigned word_cnt = 0;

  EmptyCb success_cb = nullptr;
  AckCb ack_cb = nullptr;
  DataCb data_cb = nullptr;
  ErrorCb error_cb = nullptr;
};

}

#endif
