package rfidsimpp.radio;

simple TransceiverBase {
parameters:
    @class(rfidsim::TransceiverBase);    
gates:
    input logicInp;
    output logicOut;
    input radioInp @directIn;
}

simple ActiveTransceiver extends TransceiverBase {
parameters:
    @class(rfidsim::ActiveTransceiver);
}

simple PassiveTransceiver extends TransceiverBase {
parameters:
    @class(rfidsim::PassiveTransceiver);
}
