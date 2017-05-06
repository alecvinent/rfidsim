cplusplus {{
#include <logic/reader-tagops.h>
}}

namespace rfidsim;

enum LogicError {
    NO_RESPONSE = 0;
    CHANNEL_ERROR = 1;
    COLLISION = 2;
}

packet TagConnPkt {
    int tagID;
}

packet NewTagConn extends TagConnPkt {
    // contains tagop::TagInfo as an encapsulated packet
}

packet TagConnReq extends TagConnPkt {
    // contains tagop::Request as an encapsulated packet
}

packet TagConnConf extends TagConnPkt {
    // contains tagop::Confirmation as an encapsulated packet
}

packet CloseTagConn extends TagConnPkt {
}

packet TagConnLost extends TagConnPkt {
}
