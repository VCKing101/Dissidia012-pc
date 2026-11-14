/* Runs the part's update, then refreshes the icon picked for this mode. */
class PickIconBase {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
};

class PickIconPart : public PickIconBase {
public:
    char pad_04[0x8];
    void *field_0C[16];
};

extern "C" void func_088D9CD8(void *icon);

extern "C" {
void func_08A536D8(PickIconPart *self, int mode)
{
    self->v5();
    int icons[5] = { 3, 4, 5, 6, 7 };
    func_088D9CD8(self->field_0C[icons[mode]]);
}
}
