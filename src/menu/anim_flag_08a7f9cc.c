typedef struct FlagTarget {
    char pad_000[0x4E4];
    int field_4E4;
    char pad_4E8[0x5E8 - 0x4E8];
    unsigned int field_5E8;
} FlagTarget;

typedef struct FlagOwner {
    char pad_00[0x28];
    FlagTarget *field_28;
} FlagOwner;

extern void func_088D9F5C(int handle, int anim, char flag);
extern void func_08A7F96C(FlagOwner *self, unsigned int a, unsigned char b, int c);
typedef struct AnimGlobal {
    char pad_00[4];
    unsigned short field_04;
} AnimGlobal;

extern AnimGlobal D_08A85B6A;

void func_08A7F9CC(FlagOwner *self, int a, unsigned char b, int c, int mode)
{
    FlagTarget *target;
    func_08A7F96C(self, a, b, c);
    target = self->field_28;
    if (mode == 1) {
        target->field_5E8 = target->field_5E8 | 0x8000;
        func_088D9F5C(self->field_28->field_4E4, D_08A85B6A.field_04, 1);
    } else {
        target->field_5E8 = target->field_5E8 & ~0x8000;
    }
}
