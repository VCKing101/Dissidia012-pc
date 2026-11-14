typedef struct TimerState {
    char pad_0000[0x88];
    char field_88[0x400];
    char field_488[0x604];
    char field_A8C[0x3238 - 0xA8C];
    unsigned int field_3238;
    int field_323C;
    int field_3240;
} TimerState;

typedef struct TimerOwner {
    char pad_000[0x14C];
    TimerState *field_14C;
} TimerOwner;

extern char D_08A0A3E2[];
extern char D_08A0A40A[];
extern void func_088D3F00(char *dst, char *src, char *name);
extern void func_088D44EC(char *dst, char *src, char *name);
extern void func_089F91F0(TimerOwner *self);

void func_089F914C(TimerOwner *self)
{
    if ((self->field_14C->field_3238 & 8) == 0) {
        return;
    }
    func_088D3F00(self->field_14C->field_88, self->field_14C->field_488, D_08A0A3E2);
    func_088D44EC(self->field_14C->field_488, self->field_14C->field_A8C, D_08A0A40A);
    if (self->field_14C->field_3240 != 0) {
        self->field_14C->field_3240--;
        if (self->field_14C->field_3240 == 0) {
            self->field_14C->field_3240 = 0;
            func_089F91F0(self);
        }
    }
}
