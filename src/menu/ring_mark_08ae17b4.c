typedef struct RingState {
    char pad_000[0x400];
    int field_400[0x14];
} RingState;

typedef struct RingOwner {
    char pad_000[0x850];
    RingState *field_850;
} RingOwner;

extern short D_08AE457E[];
extern void func_088D9F5C(int target, int kind, int value);

void func_08AE17B4(RingOwner *self, int index, int selected)
{
    func_088D9F5C(self->field_850->field_400[0x13 - index], D_08AE457E[selected], 1);
}
