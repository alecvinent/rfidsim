cplusplus {{
#include <protocol/data-types.h>
#include <protocol/commands.h>
#include <protocol/responses.h>
#include <common/units.h>
}}

namespace rfidsim;

class ReaderPreamble;
class TagPreamble;
class Decibel extends cObject;

packet AirFrame {
    simtime_t bodyDuration;
    Decibel modulationLoss;
}

packet ReaderAirFrame extends AirFrame {
    ReaderPreamble preamble;    
    // encapsulated - a command
}

packet TagAirFrame extends AirFrame {
    TagPreamble preamble;
    // encapsulated - a reply
}
