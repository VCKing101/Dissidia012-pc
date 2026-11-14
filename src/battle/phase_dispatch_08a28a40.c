typedef struct PhaseTask28A40 {
    char pad_00[0x8];
    int phase;
} PhaseTask28A40;

extern int func_08A28A90(PhaseTask28A40 *self);
extern int func_08A28B98(PhaseTask28A40 *self);

int func_08A28A40(PhaseTask28A40 *self)
{
    switch (self->phase) {
    case 0:
        return func_08A28A90(self);
    case 1:
        return func_08A28B98(self);
    }
    return 0;
}
