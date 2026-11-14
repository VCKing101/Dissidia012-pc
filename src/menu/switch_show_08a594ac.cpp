/* Object whose vtable pointer sits after 8 bytes; virtual 3 answers whether it is on. */
class ShowSwitchBase {
public:
    char data_00[0x8];
    virtual int v0();
    virtual int v1();
    virtual int v2();
    virtual int v3();
};

class ShowSwitch : public ShowSwitchBase {
public:
    char pad_0C[0x30 - 0xC];
    int field_30;
};

extern "C" void func_088D9CD8(int handle);
extern "C" void func_088D9CFC(int handle);

extern "C" {
void func_08A594AC(ShowSwitch *self)
{
    if (self->v3()) {
        func_088D9CD8(self->field_30);
    } else {
        func_088D9CFC(self->field_30);
    }
}
}
