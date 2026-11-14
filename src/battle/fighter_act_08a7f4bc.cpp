/* A fighter seen through its vptr after 0x544 bytes of data; virtual N sits at vtable offset 8 * (N + 1). */
class ViewFighter {
public:
    char data_000[0x544];
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
    virtual void slot_060();
    virtual void slot_068();
    virtual void slot_070();
    virtual void slot_078();
    virtual void slot_080();
    virtual void slot_088();
    virtual void slot_090();
    virtual void slot_098();
    virtual void slot_0A0();
    virtual void slot_0A8();
    virtual void slot_0B0();
    virtual void slot_0B8();
    virtual void slot_0C0();
    virtual void slot_0C8();
    virtual void slot_0D0();
    virtual void slot_0D8();
    virtual void slot_0E0();
    virtual void slot_0E8();
    virtual void slot_0F0();
    virtual void slot_0F8();
    virtual void slot_100();
    virtual void slot_108();
    virtual void slot_110();
    virtual void slot_118();
    virtual void slot_120();
    virtual void slot_128();
    virtual void slot_130();
    virtual void slot_138();
    virtual int canAct();
};

struct FighterHolder {
    char pad_00[0x4];
    ViewFighter *fighter;
};

struct ActCtrl {
    char pad_00[0x4];
    FighterHolder *holder;
    char pad_08[0x84];
    unsigned char locked;
};

extern "C" {

extern void func_088A24D4(int a, int id, int b, int c);

/* Starts id for the fighter when it can act and the controller is not locked; -1 bad id, -2 no fighter. */
int func_08A7F4BC(ActCtrl *self, int id)
{
    if (id < 0) {
        return -1;
    }
    ViewFighter *fighter = self->holder->fighter;
    if (fighter == 0) {
        return -2;
    }
    if (!fighter->canAct()) {
        return 0;
    }
    if (self->locked == 1) {
        return 0;
    }
    func_088A24D4(0, id, 1, 0);
    return 0;
}

}
