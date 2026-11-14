typedef struct StateCtx {
    char pad_000[0x910];
    int field_910;
    char pad_914[0x92C - 0x914];
    int field_92C;
} StateCtx;

typedef struct StateOwner {
    char pad_000[0x154];
    StateCtx *field_154;
} StateOwner;

extern void func_08A80EF4(int value);
extern int func_08A9A460(StateOwner *self);

void func_08A98650(StateOwner *self)
{
    if (self->field_154->field_92C != 0) {
        func_08A80EF4(self->field_154->field_92C);
    }
    func_08A9A460(self);
    self->field_154->field_910 = 11;
}
