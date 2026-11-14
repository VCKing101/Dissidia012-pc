/* Object whose SNC vtable pointer sits at +0x00. */
class Agent4Virtual00 {
public:
    virtual void slot_008(int arg);
};

extern "C" {
void func_088145C4(Agent4Virtual00 *self, int arg) {
    self->slot_008(arg);
}

void func_088145F0(Agent4Virtual00 *self, int arg) {
    self->slot_008(arg);
}
}
