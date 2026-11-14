typedef struct VirtualEntry {
    short delta;
    void (*fn)(char *self);
} VirtualEntry;

typedef struct SetupObj {
    char pad_00[8];
    char *field_08;
    int field_0C;
    int field_10;
    int field_14;
} SetupObj;

extern void func_08A59504(SetupObj *self, int a, int b, int c);
extern void func_08A596D4(SetupObj *self, int value);
extern void func_08A59804(SetupObj *self, int value);
extern void func_08A5AA3C(SetupObj *self);

void func_08A590B0(SetupObj *self, int p2, int p3, int p4, int p5, int p6, int mode)
{
    VirtualEntry *entry;
    self->field_0C = p2;
    self->field_14 = mode;
    func_08A59804(self, p3);
    func_08A59504(self, p4, p5, p6);
    func_08A596D4(self, mode);
    func_08A5AA3C(self);
    entry = (VirtualEntry *)(self->field_08 + 0x40);
    entry->fn((char *)self + entry->delta);
    self->field_10 = 1;
}
