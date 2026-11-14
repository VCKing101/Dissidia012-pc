extern "C" {
extern void func_089E71F8(void *);
extern void func_089E7450(void *);
}

struct Pair20 { char data[0x20]; };
struct PairHolder {
    int field_00;
    Pair20 entries[2];
};

extern "C" {
void func_089E7200(PairHolder *self)
{
    for (int i = 0; i < 2; i++) {
        func_089E7450(&self->entries[i]);
    }
    func_089E71F8(self);
}
}
