/* Child whose vtable pointer sits after 0x34 bytes; virtual 1 takes a 64-bit mask. */
class PollChild {
public:
    char data_00[0x34];
    virtual int v0(unsigned long long mask);
    virtual int v1(unsigned long long mask);
};

struct PollStarter {
    char pad_00[0x30];
    char field_30[0x38];
    int field_68;
};

extern "C" unsigned long long D_08A7E9F0;
extern "C" PollChild *func_088729A0(int index);
extern "C" void func_08A772FC(char *anim, int arg);
extern "C" void func_08A77614(PollStarter *self, int state);

extern "C" {
void func_08A7E668(PollStarter *self, int arg)
{
    if (func_088729A0(0)->v1(D_08A7E9F0)) {
        func_08A772FC(self->field_30, arg);
        self->field_68 = 2;
        func_08A77614(self, 2);
    }
}
}
