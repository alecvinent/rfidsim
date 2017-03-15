package rfidsim.world.life;

simple LifeUnit {
    parameters:        
        @class(rfidsim::LifeUnit);
        @signal[DestroyThing](rfidsim::DestroyThing);
        int thingID;
}

simple LifeTimeUnit extends LifeUnit {
    parameters:
        @class(rfidsim::LifeTimeUnit);
        double lifetime @unit(s);
}
