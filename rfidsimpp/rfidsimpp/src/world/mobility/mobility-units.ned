package rfidsim.world.mobility;

simple MobilityUnit {
    parameters:
        @signal[PositionUpdated](type=rfidsim::PositionUpdated);        
        
        // Full or relative path to the mobility controller. Position would
        // be updated upon receiving UpdatePosition signals from the controller.
        string controller = default("");
        
        // Full or relative path to the master mobility unit. Position would
        // be updated upon receiving PositionUpdated signals from that unit.
        string masterUnit = default("");
        
        // "" if not bound, "soft" if position is updated only (orientation
        // is independent of the master) and "hard" if both position and
        // orientation are bound
        string masterBoundType = default("");
        
        // if true, emits PositionUpdated signals, otherwise doesn't
        bool forwardUpdates = default(true);
        
        // if true, emits DevicePositionUpdated objects instead of normal
        // PositionUpdated (includes DeviceID)
        bool isDeviceMobilityUnit = default(false);
        
        // The device identifier, must be the same for all the units inside
        // the device. This field is used if and only isDeviceMobilityUnit was
        // set to true.
        int deviceID = default(-1);
        
        double x0 @unit(m) = default(0.0m);
        double y0 @unit(m) = default(0.0m);
        double z0 @unit(m) = default(0.0m);
        double fwdX = default(1);
        double fwdY = default(0);
        double fwdZ = default(0);
        double upX = default(0);
        double upY = default(0);
        double upZ = default(1);
}

simple StationaryMobilityUnit extends MobilityUnit {
    parameters:
        @class(rfidsim::StationaryMobilityUnit);
}
