typedef struct RingState {
    char pad_000[0x5B0];
    int field_5B0;
} RingState;

typedef struct RingOwner {
    char pad_000[0x850];
    RingState *field_850;
} RingOwner;

extern int D_08B01CF8;
extern short func_08AE15AC(RingOwner *self, int id);
extern void func_08AF418C(int target, int key, int value);

void func_08AE20B0(RingOwner *self)
{
    int value = 0;
    int key;
    if (self->field_850->field_5B0 != 0) {
        value = func_08AE15AC(self, self->field_850->field_5B0);
    }
    key = self->field_850->field_5B0;
    if (key == 0) {
        key = -1;
    }
    func_08AF418C(D_08B01CF8, key, value);
}
