/* Runs the part's update, then refreshes the icon for mode 0 or 1. */
class ModeIconBase {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
};

class ModeIconPart : public ModeIconBase {
public:
    char pad_04[0x8];
    void *field_0C;
    void *field_10;
    void *field_14;
    void *field_18;
};

extern "C" void func_088D9CD8(void *icon);

extern "C" {
void func_08A5260C(ModeIconPart *self, int mode)
{
    self->v2();
    switch (mode) {
    case 0:
        func_088D9CD8(self->field_14);
        break;
    case 1:
        func_088D9CD8(self->field_18);
        break;
    }
}
}
