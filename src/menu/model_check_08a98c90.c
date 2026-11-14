/* Closes the model and moves to state 0x13 unless it is enabled (0x40 and 0x2000) and busy. */
typedef struct CheckModelState {
    char pad_0000[0x910];
    int field_910;
    char pad_0914[0x9B8 - 0x914];
    void *field_9B8;
    char pad_after[0x81B4 - 0x9B8 - 4];
    unsigned int field_81B4;
} CheckModelState;

typedef struct CheckModelPage {
    char pad_000[0x154];
    CheckModelState *field_154;
} CheckModelPage;

extern int func_08B2BC9C(void *model);
extern void func_08A9AE6C(CheckModelPage *self);

void func_08A98C90(CheckModelPage *self)
{
    unsigned int flags = self->field_154->field_81B4;
    if ((flags & 0x40) != 0) {
        if ((flags & 0x2000) != 0) {
            if (self->field_154->field_9B8 != 0) {
                if (func_08B2BC9C(self->field_154->field_9B8) == 0) {
                    return;
                }
            }
        }
    }
    func_08A9AE6C(self);
    self->field_154->field_910 = 0x13;
}
