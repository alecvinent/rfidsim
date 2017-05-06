cplusplus {{
#include <protocol/epcstd-base.h>
}}

namespace rfidsim::tagop;

enum rfidsim::epcstd::MemoryBank;

packet Request {
}

enum RequestKind {
    KIND_ACK = 100;
    KIND_READ_BANK = 101;
}

packet Acknowledge extends Request {
    @customize(true);
}

packet ReadBank extends Request {
    @customize(true);
    int bank @enum(rfidsim::epcstd::MemoryBank);
    unsigned int wordPtr;
    unsigned int wordCount;
}



packet Confirmation {
}

enum ConfirmationKind {
    KIND_SUCCESS = 200;
    KIND_FAILURE = 201;
    KIND_TAGINFO = 202;
    KIND_DATA = 203;
} 

packet Success extends Confirmation {
    @customize(true);
}

enum FailureCode {
    CHANNEL_ERROR = 0;
    NO_RESPONSE = 1;
}

packet Failure extends Confirmation {
    @customize(true);
    int code @enum(FailureCode);
}

packet TagInfo extends Confirmation {
    @customize(true);
    unsigned int pc @getter(getPC) @setter(setPC);
    abstract uint8_t epc[] @getter(getEPCByte) @setter(setEPCByte)
                           @sizeGetter(getEPCByteLength) 
                           @sizeSetter(setEPCByteLength);
}

packet Data extends Confirmation {
    @customize(true);
    abstract uint8_t data[] @getter(getDataByte) @setter(setDataByte)
                            @sizeGetter(getDataByteLength) 
                            @sizeSetter(setDataByteLength);    
}
