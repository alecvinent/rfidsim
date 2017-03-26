cplusplus {{
#include <protocol/epcstd-base.h>    
}}

namespace rfidsim::epcstd;

enum TagEncoding;
enum DivideRatio;
enum Sel;
enum Session;
enum InventoryFlag;
enum MemoryBank;

enum CommandKind {
    KIND_COMMAND_QUERY = 0;
    KIND_COMMAND_QUERY_REP = 1;
    KIND_COMMAND_ACK = 2;
    KIND_COMMAND_REQ_RN = 3;
    KIND_COMMAND_READ = 4;
}

packet Command {
    int readerID;
}

packet Query extends Command {
    @customize(true);
    kind = KIND_COMMAND_QUERY;
    int dr @enum(DivideRatio) @getter(getDR) @setter(setDR);
    int m @enum(TagEncoding);
    bool trext @getter(getTRext) @setter(setTRext);
    int sel @enum(Sel);
    int session @enum(Session);
    int target @enum(InventoryFlag);
    unsigned int q;
    unsigned int crc5 @getter(getCRC5) @setter(setCRC5);
}

packet QueryRep extends Command {
    @customize(true);
    kind = KIND_COMMAND_QUERY_REP;
    int session @enum(Session);
}

packet Ack extends Command {
    @customize(true);
    kind = KIND_COMMAND_ACK;
    unsigned int rn @getter(getRN) @setter(setRN);
}

packet ReqRN extends Command {
    @customize(true);
    kind = KIND_COMMAND_REQ_RN;
    unsigned int rn @getter(getRN) @setter(setRN);
    unsigned int crc16 @getter(getCRC16) @setter(setCRC16);
}

packet Read extends Command {
    @customize(true);
    kind = KIND_COMMAND_READ;
    int bank @enum(MemoryBank);
    unsigned int wordPtr;
    unsigned int wordCount;
    unsigned int rn @getter(getRN) @setter(setRN);
    unsigned int crc16 @getter(getCRC16) @setter(setCRC16);
}
