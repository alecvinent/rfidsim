namespace rfidsim;

// PHY receives Commands (for Reader) and Reply'es (for Tag) from the Logic
// layer and sends Reply'es and Commands respectively, so there is no need
// for additional packets besides indicating errors, timeouts, etc.

enum PhyResponseType {
    PHY_RESPONSE_OK = 0;
    PHY_RESPONSE_COLLISION = 1;
    PHY_RESPONSE_CHANNEL_ERROR = 2;
}

// The packet is sent by the Reader PHY to inform Logic layer that no Reply
// was received.
packet ReplyTimeoutInd {
    @customize(true);
    int response @enum(PhyResponseType);
}
