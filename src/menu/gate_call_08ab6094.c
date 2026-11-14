typedef struct GateObj {
    char pad_00[1];
} GateObj;

typedef struct GateTarget {
    char pad_000[0x210];
    int field_210;
} GateTarget;

extern GateTarget *func_08AB6D58(GateObj *self);
extern int func_08AB6DAC(GateObj *self);
extern int func_08AB71D4(GateObj *self, int value);

int func_08AB6094(GateObj *self)
{
    if (func_08AB6DAC(self) == 1) {
        return 0;
    }
    return func_08AB71D4(self, func_08AB6D58(self)->field_210);
}
