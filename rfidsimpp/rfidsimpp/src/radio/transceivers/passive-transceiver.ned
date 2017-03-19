package rfidsim.radio.transceivers;

simple PassiveReceiver like IReceiver {
    parameters:
        @class(rfidsim::PassiveReceiver);
        int deviceID;
    gates:
        output phyOut;
        input radioInp;
}

simple PassiveTransmitter like ITransmitter {
    parameters:
        @class(rfidsim::PassiveTransmitter);
        @signal[TxStart](type=rfidsim::TxStart);
        @signal[TxFinish](type=rfidsim::TxFinish);
        int deviceID;
        double bandwidth @unit(Hz) = default(1.2e6Hz);
        double modulationLoss @unit(dB) = default(-3dB);
    gates:
        input phyInp;
}

module PassiveTransceiver extends Transceiver {
    parameters:
        rx.typename = "rfidsim.radio.transceivers.PassiveReceiver";
        tx.typename = "rfidsim.radio.transceivers.PassiveTransmitter";
}
