package rfidsim.radio.medium;

// Medium module must be placed at the top level (below the system module).
// It provides the means for peers discovery and path loss computation.
simple Medium {
    parameters:
        @display("i=misc/cloud");       
}

simple WaveguideMedium extends Medium {
    parameters:
        @class(rfidsim::WaveguideMedium);
        double loss @unit(dB);
        double noise @unit(dBm) = default(-120dBm);
}
