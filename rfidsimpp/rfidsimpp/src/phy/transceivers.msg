cplusplus {{
#include <protocol/data-types.h>
#include <protocol/commands.h>
#include <protocol/responses.h>
}}

namespace rfidsim;

enum TagEncoding;
enum DivideRatio;

class ReaderPreamble;
class TagPreamble;


// Packet is sent by Logic to Transceiver.
packet SendDataRequest {
}

// Packet is sent by Transceiver to Logic.
packet ReceiveDataIndication {
    // encapsulated -  a command or a reply
}

// Packet is sent by Transceiver to Logic when no reply is received.
packet ReceiveTimeoutIndication {
}


//
// ==== ActiveTransceiver packets ====
//

packet SendCommandRequest extends SendDataRequest {
    ReaderPreamble preamble;
    // encapsulated - an actual command
}


//
// ==== PassiveTransceiver packets ====
//

packet SendReplyRequest extends SendDataRequest {
    TagPreamble preamble;
    // encapsulated - an actual reply 
}


//
// ==== Air Frames ====
//

packet AirFrame {
    // encapsulated - a command or reply
}

packet ReaderAirFrame extends AirFrame {
    ReaderPreamble preamble;
    // encapsulated - a command
}

packet TagAirFrame extends AirFrame {
    TagPreamble preamble;
    // encapsulated - a reply
}
