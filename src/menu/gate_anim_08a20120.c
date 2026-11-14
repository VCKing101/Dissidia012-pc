typedef struct GateState {
    char pad_00000[0x25C50];
    int field_25C50;
} GateState;

typedef struct GateOwner {
    char pad_000[0x154];
    GateState *field_154;
} GateOwner;

typedef struct GateTarget {
    char pad_00[0x94];
    int field_94;
    int field_98;
} GateTarget;

extern void func_088D9F5C(int handle, int anim, char flag);

void func_08A20120(GateOwner *self, int unused, GateTarget *target)
{
    if (self->field_154->field_25C50 == -1) {
        return;
    }
    func_088D9F5C(target->field_98, 0x23, 1);
    func_088D9F5C(target->field_94, 5, 1);
}
