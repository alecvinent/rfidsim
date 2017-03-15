package rfidsim.devices.control;

//
// This module must be put at the top-level of any device. It provides the
// means for device parts discovery, as well as registers the device when
// being initialized and de-registers when being destroyed.
// 
simple DeviceControlUnit {
    parameters:
        @signal[DeviceCreated](type=rfidsim::DeviceCreated);
        @signal[DeviceDestroyed](type=rfidsim::DeviceDestroyed);
}

simple ReaderControlUnit extends DeviceControlUnit {
    parameters:
        @class(rfidsim::ReaderControlUnit);
}

simple TagControlUnit extends DeviceControlUnit {
    parameters:
        @class(rfidsim::TagControlUnit);
}
