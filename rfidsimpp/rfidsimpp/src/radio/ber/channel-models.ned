package rfidsim.radio.ber;

moduleinterface IChannelModel {
    parameters:
        @display("i=block/filter");
}

simple ChannelModel like IChannelModel {
    parameters:
        @display("i=block/filter");
}

simple ConstantBERChannelModel extends ChannelModel {
    parameters:
        @class(rfidsim::ConstantChannel);
        double ber = default(0.0);
}

simple AWGNChannelModel extends ChannelModel {
    parameters:
        @class(rfidsim::AWGNChannelModel);
}

simple RayleighChannelModel extends ChannelModel {
    parameters:
        @class(rfidsim::RayleighChannelModel);
}
