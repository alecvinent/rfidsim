package rfidsim.radio.transceivers;
import rfidsim.radio.ber.*;

simple ActiveReceiver extends Receiver
{
    parameters:
        @class(rfidsim::ActiveReceiver);
}

simple ActiveTransmitter extends Transmitter
{
    parameters:
        @class(rfidsim::ActiveTransmitter);
}

module ActiveTransceiver
{
    parameters:
        @display("i=block/rxtx;bgb=285,150");
        int deviceID;
    gates:
        input phyInp;
        output phyOut;
        input radioInp;
    submodules:
        rx: ActiveReceiver {
            @display("p=210,70");
            deviceID = this.deviceID;
        }
        tx: ActiveTransmitter {
            @display("p=130,70");
            deviceID = this.deviceID;
        }
        chan: <> like IChannelModel {
            @display("p=50,70");
        }
    connections:
        phyInp --> tx.phyInp;
        phyOut <-- rx.phyOut;
        radioInp --> rx.radioInp;
}
