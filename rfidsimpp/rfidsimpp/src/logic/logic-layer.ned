package rfidsim.logic;

simple LogicLayer {
    parameters:
        @display("i=block/cogwheel");
        int deviceID;
}

simple ReaderLogicLayer extends LogicLayer {
    parameters:
        @class(rfidsim::ReaderLogicLayer);
        int session;   // 0, 1, 2 or 3
        int sel;       // 0=~SL, 1=SL, 2=ALL
        string target; // "A" or "B"
        int q;         // 0..15
        string m;      // "FM0", "M2", "M4", "M8"
        string dr;     // "8" or "64/3"
        bool trext;
        
        // ConstantController echoes the given parameters back without changes.
        // To test additional controllers (e.g. for tuning Q and/or M values
        // based on collisions or errors number), implement the corresponding
        // controller class, provide its full path here and define its
        // settings in controllerSettings parameter.
        string controller = 
            default("rfidsim::reader::logic::ConstantController");
        
        // ConstantController ignores any XML, giving an empty XML by default.
        xml controllerSettings = default(xml("<controller/>"));
}

simple TagLogicLayer extends LogicLayer {
    parameters:
        @class(rifdsim::TagLogicLayer);
        string epc;
        string tid;
}
