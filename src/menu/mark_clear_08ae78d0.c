/* Clears bit 0 of the flags of entry `id` (the list ends at -1), unless
   func_08AF96B0 reports the screen locked. */
struct Entry08AE78D0 {
    short id;
    unsigned short flags;
};

struct State08AE78D0 {
    char pad_0000[0x9BDC];
    struct Entry08AE78D0 field_9BDC[0x2AB];
};

struct Screen08AE78D0 {
    char pad_0000[0xA690];
    struct State08AE78D0 *field_A690;
};

extern int func_08AF96B0(void);

void func_08AE78D0(struct Screen08AE78D0 *self, unsigned short id)
{
    int i;
    if (func_08AF96B0() != 0) {
        return;
    }
    for (i = 0; i < 0x2AB; i++) {
        if (self->field_A690->field_9BDC[i].id == -1) {
            break;
        }
        if (self->field_A690->field_9BDC[i].id == id) {
            self->field_A690->field_9BDC[i].flags &= ~1;
            break;
        }
    }
}
