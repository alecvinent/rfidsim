#ifndef RADIO_TRANSCEIVERS_TRANSCEIVER_MESSAGES_H_
#define RADIO_TRANSCEIVERS_TRANSCEIVER_MESSAGES_H_

#include <radio/transceivers/transceiver-messages_m.h>

namespace rfidsim {

const char *strRecvErrorType(RecvErrorType value);

class SendCommandReq : public SendCommandReq_Base {
 public:
  static const char *NAME;

  SendCommandReq() : SendCommandReq_Base(NAME) {}
  SendCommandReq(const SendCommandReq& other) : SendCommandReq_Base(other) {}
  virtual ~SendCommandReq() {}

  virtual SendCommandReq *dup() const { return new SendCommandReq(*this); }

  SendCommandReq& operator=(const SendCommandReq& rside) {
    SendCommandReq_Base::operator =(rside);
    return *this;
  }

  virtual std::string info() const;
};

class SendReplyReq : public SendReplyReq_Base {
 public:
  static const char *NAME;

  SendReplyReq() : SendReplyReq_Base(NAME) {}
  SendReplyReq(const SendReplyReq& other) : SendReplyReq_Base(other) {}
  virtual ~SendReplyReq() {}

  virtual SendReplyReq *dup() const { return new SendReplyReq(*this); }

  SendReplyReq& operator=(const SendReplyReq& rside) {
    SendReplyReq_Base::operator=(rside);
    return *this;
  }

  virtual std::string info() const;
};

class RecvBeginInd : public RecvBeginInd_Base {
 public:
  static const char *NAME;

  RecvBeginInd() : RecvBeginInd_Base(NAME) {}
  RecvBeginInd(const RecvBeginInd& other) : RecvBeginInd_Base(other) {}
  virtual ~RecvBeginInd() {}

  virtual RecvBeginInd *dup() const { return new RecvBeginInd(*this); }

  RecvBeginInd& operator=(const RecvBeginInd& rside) {
    RecvBeginInd_Base::operator=(rside);
    return *this;
  }

  virtual std::string info() const;
};

class RecvReplyInd : public RecvReplyInd_Base {
 public:
  static const char *NAME;

  RecvReplyInd() : RecvReplyInd_Base(NAME) {}
  RecvReplyInd(const RecvReplyInd& other) : RecvReplyInd_Base(other) {}
  virtual ~RecvReplyInd() {}

  virtual RecvReplyInd *dup() const { return new RecvReplyInd(*this); }

  RecvReplyInd& operator=(const RecvReplyInd& rside) {
    RecvReplyInd_Base::operator=(rside);
    return *this;
  }

  virtual std::string info() const;
};

class RecvCommandInd : public RecvCommandInd_Base {
 public:
  static const char *NAME;

  RecvCommandInd() : RecvCommandInd_Base(NAME) {}
  RecvCommandInd(const RecvCommandInd& other) : RecvCommandInd_Base(other) {}
  virtual ~RecvCommandInd() {}

  virtual RecvCommandInd *dup() const { return new RecvCommandInd(*this); }

  RecvCommandInd& operator=(const RecvCommandInd& rside) {
    RecvCommandInd_Base::operator=(rside);
    return *this;
  }

  virtual std::string info() const;
};

class RecvErrorInd : public RecvErrorInd_Base {
 public:
  static const char *NAME;

  RecvErrorInd() : RecvErrorInd_Base(NAME) {}
  RecvErrorInd(const RecvErrorInd& other) : RecvErrorInd_Base(other) {}
  virtual ~RecvErrorInd() {}

  virtual RecvErrorInd *dup() const { return new RecvErrorInd(*this); }

  RecvErrorInd& operator=(const RecvErrorInd& rside) {
    RecvErrorInd_Base::operator=(rside);
    return *this;
  }

  virtual std::string info() const;
};

}

#endif
