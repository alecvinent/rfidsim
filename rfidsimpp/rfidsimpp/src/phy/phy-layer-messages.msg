namespace rfidsim;

// PHY receives Commands (for Reader) and Reply'es (for Tag) from the Logic
// layer and sends Reply'es and Commands respectively, so there is no need
// for additional packets besides indicating errors, timeouts, etc.

enum PhyDataConfStatus {
    PHY_DATA_CONF_OK = 0;
    PHY_DATA_CONF_COLLISION = 1;
    PHY_DATA_CONF_CHANNEL_ERROR = 2;
    PHY_DATA_CONF_NO_REPLY = 3;
}

// The packet is sent by the Reader PHY to inform Logic layer that no Reply
// was received.
packet PhyDataConf {
    @customize(true);
    int status @enum(PhyDataConfStatus);
    // if status is OK, contains encapsulated reply
}
