typedef struct RingState {
    char pad_000[0x4C4];
    int field_4C4;
} RingState;

typedef struct RingOwner {
    char pad_000[0x850];
    RingState *field_850;
} RingOwner;

extern short D_08AE457A;
extern void func_088D9CD8(int handle);
extern void func_088D9CFC(int handle);
extern void func_088D9F5C(int target, int kind, int value);
extern void func_08AE0638(RingOwner *self);

void func_08AE1748(RingOwner *self, unsigned char on)
{
    if (on == 1) {
        func_088D9CD8(self->field_850->field_4C4);
        func_088D9F5C(self->field_850->field_4C4, D_08AE457A, 1);
        func_08AE0638(self);
    } else {
        func_088D9CFC(self->field_850->field_4C4);
    }
}
