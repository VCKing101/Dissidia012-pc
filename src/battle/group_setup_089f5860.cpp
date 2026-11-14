extern "C" {
extern void func_089F5808(void *);
extern void func_089F5140(void *, int, void *, void *);
}

struct Entry48 { char data[0x48]; };
struct Group5860 {
    unsigned char field_00;
    char pad_001[3];
    Entry48 entries[3];
};

extern "C" {
int func_089F5860(Group5860 *self, void *owner, void *peer)
{
    func_089F5808(self);
    if (!owner || !peer) {
        return -1;
    }
    for (int i = 0; i < 3; i++) {
        func_089F5140(&self->entries[i], i, owner, peer);
    }
    self->field_00 = 1;
    return 0;
}
}
