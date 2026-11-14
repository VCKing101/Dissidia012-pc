/* Child whose vtable pointer sits after 0x34 bytes; virtual 1 takes a 64-bit mask. */
class PollChild {
public:
    char data_00[0x34];
    virtual int v0(unsigned long long mask);
    virtual int v1(unsigned long long mask);
};

struct PollPanel {
    char pad_0000[0x10EC];
    PollChild *field_10EC;
};

extern "C" unsigned long long D_08A58688;
extern "C" void func_088D4BF4(int sound);
extern "C" void func_08A5776C(PollPanel *self);

extern "C" {
void func_08A57CB8(PollPanel *self)
{
    if (self->field_10EC != 0) {
        if (self->field_10EC->v1(D_08A58688) != 0) {
            func_088D4BF4(2);
            func_08A5776C(self);
        }
    }
}
}
