package rfidsim.phy;

simple Phy {
    parameters:
        @display("i=block/mac");
        int deviceID;
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
        double frt;       
}

simple TagPhy extends Phy {
    parameters:
        @class(rfidsim::TagPhyLayer);
}
