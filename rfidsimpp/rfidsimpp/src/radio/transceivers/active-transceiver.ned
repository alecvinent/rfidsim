package rfidsim.radio.transceivers;

simple ActiveReceiver like IReceiver {
    parameters:
        @class(rfidsim::ActiveReceiver);
        int deviceID;
    gates:
        output phyOut;
        input radioInp;
}

simple ActiveTransmitter like ITransmitter {
    parameters:
        @class(rfidsim::ActiveTransmitter);
        @signal[TxStart](type=rfidsim::TxStart);
        @signal[TxFinish](type=rfidsim::TxFinish);
        int deviceID;
    gates:
        input phyInp;
}

module ActiveTransceiver extends Transceiver {
    parameters:
        rx.typename = "rfidsim.radio.transceivers.ActiveReceiver";
        tx.typename = "rfidsim.radio.transceivers.ActiveTransmitter";
}
