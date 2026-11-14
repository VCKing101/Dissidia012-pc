typedef struct RingState {
    char pad_000[0x4AC];
    int field_4AC;
} RingState;

typedef struct RingOwner {
    char pad_000[0x850];
    RingState *field_850;
} RingOwner;

extern short D_08AE463E[];
extern void func_088D9F5C(int target, int kind, int value);

void func_08AE2804(RingOwner *self, int index)
{
    func_088D9F5C(self->field_850->field_4AC, D_08AE463E[index], 1);
}
