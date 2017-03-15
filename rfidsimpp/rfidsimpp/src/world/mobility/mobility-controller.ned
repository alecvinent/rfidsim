package rfidsim.world.mobility;

simple MobilityController {
    parameters:
        @class(rfidsim::MobilityController);
        @signal[UpdatePosition](type=rfidsim::UpdatePosition);        
        double interval;
}
