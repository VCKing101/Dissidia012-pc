typedef struct RingState {
    char pad_000[0x580];
    int field_580;
    int field_584;
    char pad_588[0x5A4 - 0x588];
    unsigned int field_5A4;
} RingState;

typedef struct RingOwner {
    char pad_000[0x850];
    RingState *field_850;
} RingOwner;

extern short D_08AE45CE;
extern short D_08AE45B4[];
extern void func_08AE1044(RingOwner *self, int id);
extern void func_08AE0C94(RingOwner *self, int part, int frame, int value);
extern void func_0887B944(int handle);

void func_08AE1A08(RingOwner *self, unsigned char on, int index)
{
    if (on == 1) {
        func_08AE1044(self, 0xC);
        if ((self->field_850->field_5A4 & 0x20) != 0) {
            func_08AE0C94(self, 0xD, D_08AE45CE, -1);
            func_08AE0C94(self, 0xC, D_08AE45CE, -1);
        } else {
            func_08AE0C94(self, 0xD, D_08AE45B4[index], -1);
            func_08AE0C94(self, 0xC, D_08AE45B4[index], -1);
        }
    } else {
        func_0887B944(self->field_850->field_580);
        func_0887B944(self->field_850->field_584);
    }
}
