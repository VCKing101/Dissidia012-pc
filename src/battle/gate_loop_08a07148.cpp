extern "C" {
extern void func_08A03B60(void *);
}

struct GateEntryB0 { char data[0xB0]; };
struct Gate7148 {
    unsigned char field_00;
    char pad_001[0x223];
    GateEntryB0 entries[2];
};

extern "C" {
int func_08A07148(Gate7148 *self)
{
    if (!self->field_00) {
        return -1;
    }
    for (int i = 0; i < 2; i++) {
        func_08A03B60(&self->entries[i]);
    }
    return 0;
}
}
