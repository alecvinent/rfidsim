package rfidsim.radio.transceivers;

simple Receiver {
    parameters:
        @display("i=block/rx");
        int deviceID;
    gates:
        output phyOut;
        input radioInp;
}

simple Transmitter {
    parameters:
        @display("i=block/tx");
        @signal[TxStart](type=rfidsim::TxStart);
        @signal[TxFinish](type=rfidsim::TxFinish);
        int deviceID;
        double bandwidth @unit(Hz);
    gates:
        input phyInp;
}
