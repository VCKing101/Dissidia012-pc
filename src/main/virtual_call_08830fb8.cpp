/* Object whose SNC vtable pointer sits at +0x00. */
class Agent4Virtual00 {
public:
    virtual void slot_008(int arg);
};

extern "C" {
void func_08830FB8(Agent4Virtual00 *self, int arg) {
    self->slot_008(arg);
}

void func_08830FE4(Agent4Virtual00 *self, int arg) {
    self->slot_008(arg);
}
}
