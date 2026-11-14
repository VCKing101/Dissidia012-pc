/* Object whose SNC vtable pointer sits at +0x08. */
class Agent4Virtual08 {
public:
    int field_00;
    int field_04;
    virtual void slot_008(int arg);
};

extern "C" {
void func_0880444C(Agent4Virtual08 *self, int arg) {
    self->slot_008(arg);
}
}
