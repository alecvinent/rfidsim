package rfidsim.radio.ber;

simple ChannelModel {
}

simple ConstantChannel extends ChannelModel {
    parameters:
        @class(rfidsim::ConstantChannel);
        double ber = default(0.0);
}
