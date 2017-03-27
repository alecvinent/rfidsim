package rfidsim.phy;

simple Phy {
    parameters:
        @display("i=block/mac");
        int deviceID;
        bool extendedTemp = default(false);
    gates:
        input logicInp;
        output logicOut;
        input radioInp;
        output radioOut;
}

simple ReaderPhy extends Phy {
    parameters:
        @class(rfidsim::ReaderPhyLayer);
        double delimiter @unit(s);
        double tari @unit(s);
        double rtcal @unit(s);
        double trcal @unit(s); 
}

simple TagPhy extends Phy {
    parameters:
        @class(rfidsim::TagPhyLayer);
}
