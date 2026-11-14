typedef struct SendObj {
    char pad_00[0x24];
    int field_24;
} SendObj;

extern int func_08A6FC24(int target, int kind, short *data, int size);
extern int func_08AB6D58(SendObj *self);
extern int func_08AB6DAC(SendObj *self);

void func_08AB5614(SendObj *self)
{
    int target;
    short value[2];
    if (func_08AB6DAC(self) == 1) {
        return;
    }
    target = func_08AB6D58(self);
    value[0] = (short)self->field_24;
    func_08A6FC24(target, 0x11, value, 2);
}
