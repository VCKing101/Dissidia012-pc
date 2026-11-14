extern "C" {
extern void func_089F5098(void *);
extern void func_089F57C8(void *);
}

struct Entry48 { char data[0x48]; };
struct Group5808 {
    int field_00;
    Entry48 entries[3];
};

extern "C" {
void func_089F5808(Group5808 *self)
{
    for (int i = 0; i < 3; i++) {
        func_089F5098(&self->entries[i]);
    }
    func_089F57C8(self);
}
}
