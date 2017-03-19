package rfidsim.radio.transceivers;

moduleinterface IReceiver {
    parameters:
        int deviceID;
    gates:
        output phyOut;
        input radioInp;
}

moduleinterface ITransmitter {
    parameters:
        @signal[TxStart](type=rfidsim::TxStart);
        @signal[TxFinish](type=rfidsim::TxFinish);
        int deviceID;
        double bandwidth @unit(Hz);
    gates:
        input phyInp;
}

module Transceiver {
    parameters:
        int deviceID;
    gates:
        input phyInp;
        output phyOut;
        input radioInp;
        
    submodules:
        rx: <> like IReceiver {
            deviceID = this.deviceID;
        }
        tx: <> like ITransmitter {
            deviceID = this.deviceID;
        }
    connections:
        phyInp --> tx.phyInp;
        phyOut <-- rx.phyOut;
        radioInp --> rx.radioInp;
}
