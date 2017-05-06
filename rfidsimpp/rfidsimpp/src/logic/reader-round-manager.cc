#include <logic/reader-round-manager.h>
#include <logic/reader-logic-layer.h>

using namespace omnetpp;

namespace rfidsim {

using namespace epcstd;

const char *ReaderRoundManager::str(State state)
{
  switch (state) {
    case OFF: return "OFF";
    case READY: return "READY";
    case WAIT_HANDLE: return "WAIT_HANDLE";
    case WAIT_RESULT: return "WAIT_RESULT";
    default: throw cRuntimeError("unrecognized State = %d", state);
  }
}

const char *ReaderRoundManager::str(Operation operation)
{
  switch (operation) {
    case NONE: return "None";
    case QUERY_TAGS: return "Query Tags";
    case ACKNOWLEDGE: return "Acknowledge";
    case READ_BANK: return "Read Bank";
    default: throw cRuntimeError("unrecognized Operation = %d", operation);
  }
}

ReaderRoundManager::ReaderRoundManager(
        ReaderLogicLayer *logic_module, NewTagCb new_tag_cb, DataCb data_cb,
        EmptyCb success_cb, ErrorCb error_cb)
: logic_module(logic_module), success_cb(success_cb), ack_cb(new_tag_cb),
  data_cb(data_cb), error_cb(error_cb)
{
  // nothing to be done here
}


ReaderRoundManager::~ReaderRoundManager()
{
  // nothing to be done here
}

void ReaderRoundManager::start(const RoundDescriptor& descr)
{
  if (state == OFF)
  {
    setState(READY);
    setOperation(QUERY_TAGS);
    round_descriptor = descr;

    Query *query = new Query;
    query->setDR(round_descriptor.dr);
    query->setM(round_descriptor.m);
    query->setTRext(round_descriptor.trext);
    query->setSel(round_descriptor.sel);
    query->setSession(round_descriptor.session);
    query->setTarget(round_descriptor.target);
    query->setQ(round_descriptor.q);
    query->setCRC5(0x00);  // FIXME

    sendCmd(query);
    setState(WAIT_HANDLE);
  }
}

void ReaderRoundManager::stop()
{
  if (state != OFF)
  {
    setState(OFF);
    setOperation(NONE);
    handle = 0x0000;
  }
}

void ReaderRoundManager::nextSlot()
{
  if (state != READY)
    throw cRuntimeError("nextSlot() while state='%s', op='%s'", str(state),
                        str(operation));
  setOperation(QUERY_TAGS);

  QueryRep *query_rep = new QueryRep;
  query_rep->setSession(round_descriptor.session);

  sendCmd(query_rep);
  setState(WAIT_HANDLE);
}

void ReaderRoundManager::acknowledge()
{
  if (state != READY)
    throw cRuntimeError("ackowledge() while in state='%s', op='%s'",
                        str(state), str(operation));
  setOperation(ACKNOWLEDGE);

  Ack *ack = new Ack;
  ack->setRN(handle);

  sendCmd(ack);
  setState(WAIT_RESULT);
}

void ReaderRoundManager::readBank(
        MemoryBank bank, unsigned word_ptr, unsigned word_cnt)
{
  if (state != READY)
    throw cRuntimeError("readBank() while in state='%s', op='%s'", str(state),
                        str(operation));
  setOperation(READ_BANK);

  ReqRN *req_rn = new ReqRN;
  req_rn->setRN(handle);
  req_rn->setCRC16(0x0000);  // FIXME

  sendCmd(req_rn);
  setState(WAIT_HANDLE);
}

void ReaderRoundManager::handlePhyDataConf(PhyDataConf *msg)
{
  auto status = static_cast<PhyDataConfStatus>(msg->getStatus());
  if (status == PHY_DATA_CONF_OK)
  {
    auto reply = dynamic_cast<Reply*>(msg->decapsulate());
    if (reply)
    {
      auto kind = reply->getKind();
      if (kind == KIND_QUERY_REPLY)
        processQueryReply(check_and_cast<QueryReply*>(reply));
      else if (kind == KIND_REQ_RN_REPLY)
        processReqRNReply(check_and_cast<ReqRNReply*>(reply));
      else if (kind == KIND_ACK_REPLY)
        processAckReply(check_and_cast<AckReply*>(reply));
      else if (kind == KIND_READ_REPLY)
        processReadReply(check_and_cast<ReadReply*>(reply));
      else
        throw cRuntimeError("unrecognized epcstd::ReplyKind = %d", kind);
    }
    else
    {
      processNoReplySuccess();
    }
  }
  else
  {
    processPhyError(status);
  }
  delete msg;
}

void ReaderRoundManager::processNoReplySuccess()
{
  success_cb();
}

void ReaderRoundManager::processQueryReply(QueryReply *msg)
{
  if (state != WAIT_HANDLE || operation != QUERY_TAGS)
    throw cRuntimeError("unexpected QueryReply in state='%s', op='%s'",
                        str(state), str(operation));
  handle = msg->getRN16();

  Ack *ack = new Ack;
  ack->setRN(handle);

  sendCmd(ack);
  setState(WAIT_RESULT);

  delete msg;
}

void ReaderRoundManager::processAckReply(AckReply *msg)
{
  if (state != WAIT_RESULT ||
          (operation != QUERY_TAGS && operation != ACKNOWLEDGE))
    throw cRuntimeError("unexpected AckReply in state='%s' op='%s'", str(state),
                        str(operation));

  setState(READY);
  setOperation(NONE);

  ack_cb(msg->getTagID(), msg->getPC(), msg->getEPC());

  delete msg;
}

void ReaderRoundManager::processReqRNReply(ReqRNReply *msg)
{
  if (state != WAIT_HANDLE || operation != READ_BANK)
    throw cRuntimeError("unexpected ReqRNReply in state='%s', op='%s'",
                        str(state), str(operation));

  if (operation == READ_BANK)
  {
    setState(WAIT_RESULT);

    Read *read_cmd = new Read;
    read_cmd->setBank(bank);
    read_cmd->setWordPtr(word_ptr);
    read_cmd->setWordCount(word_cnt);
    read_cmd->setRN(handle);
    read_cmd->setCRC16(0x0000);  // FIXME

    sendCmd(read_cmd);
    setState(WAIT_RESULT);
  }

  delete msg;

}

void ReaderRoundManager::processReadReply(ReadReply *msg)
{
  if (state != WAIT_RESULT || operation != READ_BANK)
    throw cRuntimeError("unexpected ReadReply in state='%s', op='%s'",
                        str(state), str(operation));

  setState(READY);
  setOperation(NONE);

  data_cb(msg->getMem());

  delete msg;
}

void ReaderRoundManager::processPhyError(PhyDataConfStatus status)
{
  if (state != WAIT_RESULT && state != WAIT_HANDLE)
    throw cRuntimeError("unexpected PhyDataConf in state='%s', op='%s'",
                        str(state), str(operation));

  setState(READY);
  setOperation(NONE);

  if (status == PHY_DATA_CONF_NO_REPLY)
    error_cb(NO_RESPONSE);
  else if (status == PHY_DATA_CONF_CHANNEL_ERROR)
    error_cb(CHANNEL_ERROR);
  else if (status == PHY_DATA_CONF_COLLISION)
    error_cb(COLLISION);
  else
    throw cRuntimeError("unrecognized PhyDataConfStatus = %d", status);
}

void ReaderRoundManager::setState(State state)
{
  this->state = state;
}

void ReaderRoundManager::setOperation(Operation operation)
{
  this->operation = operation;
}

void ReaderRoundManager::sendCmd(epcstd::Command *cmd)
{
  auto phy_out = logic_module->getPhyOut();
  logic_module->send(cmd, phy_out);
}

}
