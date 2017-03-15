cplusplus {{
#include <common/units.h>
#include <protocol/commands.h>
#include <protocol/responses.h>
}}

namespace rfidsim;

class Power;
class ReaderPreamble;
class TagPreamble;

packet SendDataReq {
}

packet SendCommandReq extends SendDataReq {
    ReaderPreamble preamble;
    // encapsulated - a Command
}

packet SendReplyReq extends SendDataReq {
    TagPreamble preamble;
    // encapsulated - a Response
}

packet RecvBeginInd {
}

packet RecvDataInd {
}

packet RecvReplyInd extends RecvDataInd {
    TagPreamble preamble;
    // encapsulated - a Response
}

packet RecvCommandInd extends RecvDataInd {
    ReaderPreamble preamble;
    // encapsulated - a Command
}

enum RecvErrorType {
    RECV_ERROR_LOW_SNR = 0;
    RECV_ERROR_COLLISION = 1;
}

packet RecvErrorInd {
    int type @enum(RecvErrorType);
}
