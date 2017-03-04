# RFID Simulator

Libraries for UHF RFID simulation in OMNeT++ and Python 3.

## Project Structure

The project consists of four layers:

1. User API -- the top layer with Jupyter notebooks for user interaction,
unifies access to the lower layers.

2. World Layer -- different devices and objects, adapters and bindings to
external models are living here, e.g. vehicles and network bridges.

3. Logical Layer -- this layer provides models of UHF RFID logical layer

4. Physical Layer -- the layer provides models of UHF RFID physical layer

### User API

### World Layer

### Logical Layer

### Physical Layer

The layer provides OMNeT++ and PyONS active and passive transceivers models.

Three key entities here:

- Active Transceiver (Reader physical layer)

- Passive Transceiver (Tag physical layer)

- Channel - an entity uniting active and passive transceivers for signal
propagation modeling.

#### Active Transceiver

The model of UHF Reader physical layer. 
