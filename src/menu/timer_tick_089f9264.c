typedef struct TimerState {
    char pad_0000[0x88];
    char field_88[0x400];
    char field_488[0x3238 - 0x488];
    unsigned int field_3238;
    int field_323C;
    int field_3240;
    int field_3244;
} TimerState;

typedef struct TimerOwner {
    char pad_000[0x14C];
    TimerState *field_14C;
} TimerOwner;

extern char D_08A0A45A[];
extern void func_088D3F00(char *dst, char *src, char *name);
extern void func_08A043E4(TimerOwner *self);

void func_089F9264(TimerOwner *self)
{
    if ((self->field_14C->field_3238 & 0x10) == 0) {
        return;
    }
    func_088D3F00(self->field_14C->field_88, self->field_14C->field_488, D_08A0A45A);
    if (self->field_14C->field_3244 != 0) {
        self->field_14C->field_3244--;
        if (self->field_14C->field_3244 == 0) {
            self->field_14C->field_3244 = 0;
            func_08A043E4(self);
        }
    }
}
