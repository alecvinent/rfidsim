package rfidsim.power;

simple PowerUnit {
    parameters:
        @class(rfidsim::PowerUnit);
        @signal[powerUpdate](type=rfidsim::PowerUpdate);
        @signal[powerOn](type=rfidsim::PowerOn);
        @signal[powerOff](type=rfidsim::PowerOff);
        int deviceID;
}

simple PassivePowerUnit extends PowerUnit {
    parameters:
        @class(rfidsim::PassivePowerUnit);
        double sensitivity @unit(dBm);
}

simple ActivePowerUnit extends PowerUnit {
    parameters:
        @class(rfidsim::ActivePowerUnit);
        double powerOnDuration @unit(s);
        double powerOffDuration @unit(s);
        double maxPower @unit(dBm) = default(31.5dBm);
}
