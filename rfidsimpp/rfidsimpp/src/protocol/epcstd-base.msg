namespace rfidsim::epcstd;

enum CommandType {
    COMMAND_QUERY = 0;
    COMMAND_QUERY_REP = 1;
    COMMAND_ACK = 2;
    COMMAND_REQ_RN = 3;
    COMMAND_READ = 4;
}

enum TagEncoding {
    FM_0 = 1;
    MILLER_2 = 2;
    MILLER_4 = 4;
    MILLER_8 = 8;
}

enum DivideRatio {
    DR_8 = 0;
    DR_64_3 = 1;
}

enum Sel {
    SEL_ALL = 0;
    SEL_NOT = 1;
    SEL_YES = 2;
}

enum Session {
    SESSION_0 = 0;
    SESSION_1 = 1;
    SESSION_2 = 2;
    SESSION_3 = 3;
}

enum InventoryFlag {
    FLAG_A = 0;
    FLAG_B = 1;
}

enum MemoryBank {
    BANK_EPC = 0;
    BANK_TID = 1;
    BANK_USER = 2;
}
