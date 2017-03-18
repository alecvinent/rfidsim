package rfidsim.logic;

simple LogicLayer {
    parameters:
        @display("i=block/cogwheel");
        int deviceID;
}

simple ReaderLogicLayer extends LogicLayer {
    parameters:
        @class(rfidsim::ReaderLogicLayer);
        int q;
        string m;   // "FM0", "M2", "M4", "M8"
        string dr;  // "8" or "64/3"
        bool trext;
        
        bool readTID = default(true);
        int tidWordPtr = default(0);
        int tidWordCount = default(4);
}

simple TagLogicLayer extends LogicLayer {
    parameters:
        @class(rifdsim::TagLogicLayer);
        string epc;
        string tid;
}
