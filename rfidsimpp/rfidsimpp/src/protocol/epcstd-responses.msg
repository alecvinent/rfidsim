cplusplus {{
#include <protocol/epcstd-data-types.h>
#include <cstdint>
}}

namespace rfidsim::epcstd;

enum ReplyKind {
    KIND_QUERY_REPLY = 0;
    KIND_ACK_REPLY = 1;
    KIND_REQ_RN_REPLY = 2;
    KIND_READ_REPLY = 3;
}

packet Reply {
    int tagID;
}

packet QueryReply extends Reply {
    @customize(true);
    unsigned int rn16 @getter(getRN16) @setter(setRN16);
}

packet AckReply extends Reply {
    @customize(true);
    unsigned int pc @getter(getPC) @setter(setPC);
    abstract uint8_t epc[] @getter(getEPCByte) @setter(setEPCByte)
                           @sizeGetter(getEPCByteLength) 
                           @sizeSetter(setEPCByteLength);
    unsigned int crc @getter(getCRC) @setter(setCRC);
}

packet ReqRNReply extends Reply {
    @customize(true);
    unsigned int rn @getter(getRN) @setter(setRN);
    unsigned int crc16 @getter(getCRC16) @setter(setCRC16);
}

packet ReadReply extends Reply {
    @customize(true);
    abstract uint8_t mem[] @getter(getMemByte) @setter(setMemByte)
                           @sizeGetter(getMemByteLength) 
                           @sizeSetter(setMemByteLength);
    unsigned int rn @getter(getRN) @setter(setRN);
    unsigned int crc16 @getter(getCRC16) @setter(setCRC16);
}
