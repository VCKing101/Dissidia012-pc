typedef struct GateState {
    char pad_0000[0x240C];
    int field_240C;
} GateState;

typedef struct GateOwner {
    char pad_000[0x28F0];
    GateState *field_28F0;
} GateOwner;

typedef struct GateTarget {
    char pad_000[0x168];
    int field_168;
    int field_16C;
} GateTarget;

extern void func_088D9F5C(int handle, int anim, char flag);

void func_08A253A0(GateOwner *self, int unused, GateTarget *target)
{
    int result = -1;
    if (self->field_28F0->field_240C == result) {
        return;
    }
    func_088D9F5C(target->field_16C, 0x23, 1);
    func_088D9F5C(target->field_168, 5, 1);
}
