package rfidsim.radio.transceivers;
import rfidsim.radio.ber.*;

simple PassiveReceiver extends Receiver {
    parameters:
        @class(rfidsim::PassiveReceiver);
}

simple PassiveTransmitter extends Transmitter {
    parameters:
        @class(rfidsim::PassiveTransmitter);
        double modulationLoss @unit(dB);
}

module PassiveTransceiver {
    parameters:
        @display("i=block/rxtx;bgb=285,150");
        int deviceID;
    gates:
        input phyInp;
        output phyOut;
        input radioInp;
    submodules:
        rx: PassiveReceiver {
            @display("p=210,70");
            deviceID = this.deviceID;
        }
        tx: PassiveTransmitter {
            @display("p=130,70");
            deviceID = this.deviceID;
        }
        chan: ConstantBERChannelModel {
            @display("p=50,70");
            ber = default(0.0);
        }
    connections:
        phyInp --> tx.phyInp;
        phyOut <-- rx.phyOut;
        radioInp --> rx.radioInp;
}
