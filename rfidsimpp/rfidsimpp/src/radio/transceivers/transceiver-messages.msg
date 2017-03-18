cplusplus {{
#include <common/units.h>
#include <protocol/epcstd-commands.h>
#include <protocol/epcstd-responses.h>
}}

namespace rfidsim;

class Power;
class epcstd::ReaderPreamble;
class epcstd::TagPreamble;
class Decibel extends cObject;

packet SendDataReq {
}

packet SendCommandReq extends SendDataReq {
    @customize(true);
    epcstd::ReaderPreamble preamble;
    // encapsulated - a Command
}

packet SendReplyReq extends SendDataReq {
    @customize(true);
    epcstd::TagPreamble preamble;
    // encapsulated - a Response
}

packet RecvBeginInd {
    @customize(true);
}

packet RecvDataInd {
}

packet RecvReplyInd extends RecvDataInd {
    @customize(true);
    epcstd::TagPreamble preamble;
    // encapsulated - a Response
}

packet RecvCommandInd extends RecvDataInd {
    @customize(true);
    epcstd::ReaderPreamble preamble;
    // encapsulated - a Command
}

enum RecvErrorType {
    RECV_ERROR_LOW_SNR = 0;
    RECV_ERROR_COLLISION = 1;
}

packet RecvErrorInd {
    @customize(true);
    int type @enum(RecvErrorType);
}


packet AirFrame {
    simtime_t preambleDuration;
    simtime_t bodyDuration;
    Decibel modulationLoss;
    double bandwidth;
    // encapsualted - a command or a reply, see derived classes
}

packet ReaderAirFrame extends AirFrame {
    @customize(true);
    epcstd::ReaderPreamble preamble;
    // encapsulated - a command
}

packet TagAirFrame extends AirFrame {
    @customize(true);
    epcstd::TagPreamble preamble;
    // encapsulated - a reply
}
