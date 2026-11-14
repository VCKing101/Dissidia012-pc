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
};

struct Agent4Task226BC {
    unsigned char unknown_0000[0x20];
    union {
        unsigned int flags;
        struct {
            unsigned int active : 1;
        } bits;
    } state;
    Agent4Virtual544 *owner;
};

extern "C" {
void func_08823660(Agent4Task226BC *self);
void func_08823870(Agent4Task226BC *self);
void func_088249EC(Agent4Task226BC *self);
void func_088242EC(Agent4Task226BC *self);
void func_088243D0(Agent4Task226BC *self);
void func_088244F4(Agent4Task226BC *self);

int func_088226BC(Agent4Task226BC *self) {
    if (!self->state.bits.active) {
        return -1;
    }
    self->owner->slot_0E8();
    func_08823660(self);
    func_08823870(self);
    func_088249EC(self);
    func_088242EC(self);
    func_088243D0(self);
    func_088244F4(self);
    return 0;
}
}
