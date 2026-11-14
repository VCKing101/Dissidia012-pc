typedef struct OpenPage {
    char pad_000[0x59C];
    int field_59C;
} OpenPage;

typedef struct OpenOwner {
    char pad_0000[0x1320];
    OpenPage *field_1320;
} OpenOwner;

extern void func_08A0C990(OpenOwner *self);
extern void func_08A0EBC8(OpenOwner *self, unsigned char flag);
extern void func_088D9F5C(int target, int kind, int value);

void func_08A0C904(OpenOwner *self, unsigned char flag)
{
    func_08A0C990(self);
    func_08A0EBC8(self, flag);
    func_088D9F5C(self->field_1320->field_59C, 0x16, 1);
}
