typedef struct VirtualEntry {
    short delta;
    void (*fn)(char *self);
} VirtualEntry;

typedef struct VirtualHost {
    char pad_00[196];
    char *field_C4;
} VirtualHost;

extern void func_08A51EB8(VirtualHost *self);

void func_08A5DFA4(VirtualHost *self)
{
    char *sub = (char *)self + 0xA0;
    VirtualEntry *entry = (VirtualEntry *)(self->field_C4 + 0x20);
    entry->fn(sub + entry->delta);
    func_08A51EB8(self);
}
