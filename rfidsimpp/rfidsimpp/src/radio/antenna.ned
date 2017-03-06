package rfidsimpp.radio;

moduleinterface Antenna {
    parameters:
        @display("i=device/receiverdish");
        double gain @unit(dBi);
        double cableLoss @unit(dB);
}

simple AntennaBase like Antenna {
    parameters:
        @display("i=device/receiverdish");
        @class(rfidsim::AntennaBase);
        double gain @unit(dBi);
        double cableLoss @unit(dB) = default(0dB);    
}

simple IsotropicAntenna extends AntennaBase {
    parameters:
        @class(rfidsim::AntennaBase);
        gain = 0.0dBi;        
}

simple DipoleAntenna extends AntennaBase {
    parameters:
        @class(rfidsim::AntennaBase);
        gain = default(8.0dBi);
}
