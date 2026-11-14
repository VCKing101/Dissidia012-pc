/* Object whose SNC vtable pointer sits at +0x544. */
class Agent4Virtual544 {
public:
    unsigned char unknown_0000[0x544];
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
    virtual void slot_110(int arg);
};

extern "C" {
void func_0880CCFC(Agent4Virtual544 *self, int arg) {
    self->slot_110(arg);
}
}
