extern "C" {
extern void func_08A039AC(void *);
}

struct GateEntryB0 { char data[0xB0]; };
struct Gate6FEC {
    unsigned char field_00;
    char pad_001[0x223];
    GateEntryB0 entries[2];
};

extern "C" {
int func_08A06FEC(Gate6FEC *self)
{
    if (!self->field_00) {
        return -1;
    }
    for (int i = 0; i < 2; i++) {
        func_08A039AC(&self->entries[i]);
    }
    return 0;
}
}
