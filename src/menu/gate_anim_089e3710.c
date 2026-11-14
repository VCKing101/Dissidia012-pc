typedef struct GateState {
    char pad_000[0xB20];
    int field_B20;
} GateState;

typedef struct GateOwner {
    char pad_000[0x140];
    GateState *field_140;
} GateOwner;

typedef struct GateTarget {
    char pad_00[0xFC];
    int field_FC;
    int field_100;
} GateTarget;

extern void func_088D9F5C(int handle, int anim, char flag);

void func_089E3710(GateOwner *self, int unused, GateTarget *target)
{
    int result = -1;
    if (self->field_140->field_B20 == result) {
        return;
    }
    func_088D9F5C(target->field_100, 0x23, 1);
    func_088D9F5C(target->field_FC, 5, 1);
}
