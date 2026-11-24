/* Returns whether both mode flags are set, every flagged or busy slot unit is
   the current owner, and the mode has a count. */
extern "C" {

struct SlotFlags {
    unsigned char b0;
    unsigned char b1;
    unsigned char bits;
};

struct SlotUnit {
    char pad_000[0x5B0];
    SlotFlags flags;
    char pad_5B3[0x784 - 0x5B3];
    char state[4];
};

struct ModeObj {
    SlotUnit **slots[2];
    char pad_08[0x58 - 8];
    unsigned char a;
    unsigned char b;
    char pad_5A[0xB4 - 0x5A];
    int count;
};

int func_08ACAA74(void *state);
SlotUnit *func_08886A60(ModeObj *self);

int func_0888696C(ModeObj *self)
{
    if (self->a == 0 || self->b == 0) {
        return 0;
    }
    for (int i = 0; i < 2; i++) {
        SlotUnit *u = *self->slots[i];
        SlotFlags *f = &u->flags;
        char *state = u->state;

        if ((unsigned char)((f->bits & 0x10) ? 1 : 0) == 1 || func_08ACAA74(state) == 1) {
            if (u != func_08886A60(self)) {
                return 0;
            }
        }
    }
    return self->count != 0;
}

}
