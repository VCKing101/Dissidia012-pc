/* Turns the wait marker (0x4000) on or off while waiting is allowed (0x2000) and not locked (0x8000). */
typedef struct WaitState {
    char pad_000[0x41C];
    unsigned int field_41C;
} WaitState;

typedef struct WaitOwner {
    char pad_000[0x30C];
    WaitState *field_30C;
} WaitOwner;

extern unsigned char D_089D5F20;
extern void func_08A89FB8(WaitOwner *self);
extern void func_08A8A010(WaitOwner *self);

void func_08A8A440(WaitOwner *self, unsigned char on, int quiet)
{
    WaitState *state;
    unsigned int flags;
    if (D_089D5F20 == 1) {
        return;
    }
    state = self->field_30C;
    flags = state->field_41C;
    if ((flags & 0x2000) == 0) {
        state->field_41C = flags & ~0x4000;
        return;
    }
    if ((flags & 0x8000) != 0) {
        return;
    }
    if (on == 1) {
        state->field_41C = flags | 0x4000;
        if (quiet == 0) {
            func_08A89FB8(self);
        }
    } else {
        state->field_41C = flags & ~0x4000;
        if (quiet == 0) {
            func_08A8A010(self);
        }
    }
}
