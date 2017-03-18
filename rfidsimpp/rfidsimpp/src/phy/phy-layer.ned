package rfidsim.phy;

simple Phy {
    parameters:
        @display("i=block/mac");
        int deviceID;
    gates:
        input logicInp;
        output logicOut;
        input transceiverInp;
        output transceiverOut;
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
