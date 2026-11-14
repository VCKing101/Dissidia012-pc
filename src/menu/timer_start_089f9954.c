typedef struct TimerState {
    char pad_0000[0x88];
    char field_88[0x400];
    char field_488[0x3238 - 0x488];
    unsigned int field_3238;
    char pad_323C[0x33AC - 0x323C];
    int field_33AC;
} TimerState;

typedef struct TimerOwner {
    char pad_000[0x14C];
    TimerState *field_14C;
} TimerOwner;

extern char D_08A0A5AA[];
extern void func_088D3CF0(char *dst, char *src, char *name);
extern void func_08A0470C(TimerOwner *self, int value);
extern void func_089F99CC(TimerOwner *self);

void func_089F9954(TimerOwner *self)
{
    if ((self->field_14C->field_3238 & 0x100) != 0) {
        return;
    }
    func_088D3CF0(self->field_14C->field_88, self->field_14C->field_488, D_08A0A5AA);
    func_08A0470C(self, self->field_14C->field_33AC);
    self->field_14C->field_3238 = self->field_14C->field_3238 | 0x100;
    func_089F99CC(self);
}
