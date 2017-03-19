package rfidsim.radio;

import rfidsim.world.mobility.MobilityUnit;

simple Antenna extends MobilityUnit {
    parameters:
        @signal[ReceivedFieldOff](type=rfidsim::ReceivedFieldOff);
        @signal[ReceivedFieldOn](type=rfidsim::ReceivedFieldOn);
        @signal[ReceivedFieldUpdated](type=rfidsim::ReceivedFieldUpdated);
        @signal[RadiatedFieldOff](type=rfidsim::RadiatedFieldOff);
        @signal[RadiatedFieldOn](type=rfidsim::RadiatedFieldOn);
        @signal[RadiatedFieldUpdated](type=rfidsim::RadiatedFieldUpdated);
        @signal[AntennaActivated](type=rfidsim::AntennaActivated);

        bool activated = default(false);
        
        double cableLoss @unit(dB);
        double gain @unit(dB);
        string polarization; // "horizontal", "vertical" or "circular"
        
        forwardUpdates = false;
    gates:
        input switchInp @loose; // this gate is used with Switch unit only
}

simple IsotropicAntenna extends Antenna {
    parameters:
        @class(rfidsim::IsotropicAntenna);
        gain = default(0dB);
}

simple DipoleAntenna extends Antenna {
    parameters:
        @class(rfidsim::DipoleAntenna);
        gain = default(8dB);
        bool blockBackLobe = default(true);
}
