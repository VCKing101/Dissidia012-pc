typedef struct RingState {
    char pad_000[0x5C0];
    int field_5C0;
} RingState;

typedef struct RingOwner {
    char pad_000[0x850];
    RingState *field_850;
} RingOwner;

extern void func_08AE17B4(RingOwner *self, int index, int selected);

void func_08AE17FC(RingOwner *self)
{
    int i;
    for (i = 0; i < 4; i++) {
        int selected = 0;
        if (self->field_850->field_5C0 == i + 1) {
            selected = 1;
        }
        func_08AE17B4(self, i, selected);
    }
}
