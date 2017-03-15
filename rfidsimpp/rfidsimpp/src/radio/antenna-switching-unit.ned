package rfidsim.radio;

simple AntennaSwitchingUnit {
    parameters:
        @class(rfidsim::AntennaSwitchingUnit);
        int deviceID;
        double interval @unit(s);
        bool switchOnlyWhenPowered = default(true);
    gates:
        output antennaOut[];  // these gates are needed to index antennas 
}
