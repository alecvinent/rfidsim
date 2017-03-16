package rfidsim.devices.topology;

simple ConnectionManager {
    parameters:
        @display("i=block/network2");
        @class(rfidsim::ConnectionManager);
        
        @signal[ConnectionCreated](type=rfidsim::ConnectionCreated);
        @signal[ConnectionLost](type=rfidsim::ConnectionLost);
        
        double connectDistance @unit(m) = default(20m);
        double disconnectDistance @unit(m) = default(25m);
}
