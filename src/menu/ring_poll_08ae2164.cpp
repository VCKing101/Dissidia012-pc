/* Child whose vtable pointer sits after 0x34 bytes; virtual 1 takes a 64-bit mask. */
class PollChild {
public:
    char data_00[0x34];
    virtual int v0(unsigned long long mask);
    virtual int v1(unsigned long long mask);
};

struct RingState {
    char pad_000[0x5A4];
    unsigned int field_5A4;
};

struct RingOwner {
    char pad_000[0x850];
    RingState *field_850;
};

extern "C" unsigned long long D_08AE4648;
extern "C" PollChild *func_088729A0(int index);
extern "C" void func_08AE21F4(RingOwner *self, int mode);

extern "C" {
void func_08AE2164(RingOwner *self)
{
    if ((self->field_850->field_5A4 & 0x100) != 0) {
        if (func_088729A0(0)->v1(D_08AE4648) != 0 && (self->field_850->field_5A4 & 0x20) == 0) {
            func_08AE21F4(self, 1);
        }
    }
}
}
