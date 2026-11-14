/* When the manager accepts the start mask, sends message 4 and resets the page parts. */
class StartManager {
public:
    char pad_00[0x34];
    virtual void v0();
    virtual int v1(unsigned long long mask);
};

struct StartOther {
    char pad_000[0x238];
    void *field_238;
};

struct StartPage {
    char pad_0000[0x4];
    char field_04[0x42C];
    char field_430[0x10];
    char field_440[0x9B60 - 0x440];
    int field_9B60;
};

extern "C" StartManager *func_088729A0(int index);
extern "C" unsigned long long D_08A7CA10;
extern "C" void func_08A6F170(void *target, int index, int arg);
extern "C" void func_08A743E0(void *part);
extern "C" void func_08A72F5C(void *part);
extern "C" void func_08A75A1C(void *part);
extern "C" void func_08A77614(StartPage *self, int state);

extern "C" {
void func_08A7C17C(StartPage *self, StartOther *other)
{
    if (func_088729A0(0)->v1(D_08A7CA10) != 0) {
        func_08A6F170(other->field_238, 4, 0);
        func_08A743E0(self->field_430);
        func_08A72F5C(self->field_440);
        func_08A75A1C(self->field_04);
        self->field_9B60 = 0;
        func_08A77614(self, 2);
    }
}
}
