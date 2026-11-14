struct FighterB534 {
    char pad_000[0x180];
    short guard;
};

class GuardHandlerB534 {
public:
    char data_000[0x8];
    virtual void slot_008();
    virtual void slot_010();
    virtual void slot_018();
    virtual void slot_020();
    virtual void slot_028();
    virtual void slot_030();
    virtual void slot_038();
    virtual void slot_040();
    virtual void slot_048();
    virtual void slot_050();
    virtual void slot_058();
    virtual void slot_060(FighterB534 *fighter);
};

extern "C" {

extern void func_08834C10(FighterB534 *fighter, int mode, int flag);

void func_08A5B534(GuardHandlerB534 *self, FighterB534 *fighter)
{
    if (fighter->guard > 0 && fighter->guard < 3) {
        func_08834C10(fighter, 0, 0);
    } else {
        self->slot_060(fighter);
    }
}

}
