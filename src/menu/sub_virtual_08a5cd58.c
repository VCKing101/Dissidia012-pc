typedef struct VirtualEntry {
    short delta;
    void (*fn)(char *self);
} VirtualEntry;

typedef struct VirtualHost {
    char pad_00[116];
    char *field_74;
} VirtualHost;

extern void func_08A51EB8(VirtualHost *self);

void func_08A5CD58(VirtualHost *self)
{
    char *sub = (char *)self + 0x50;
    VirtualEntry *entry = (VirtualEntry *)(self->field_74 + 0x20);
    entry->fn(sub + entry->delta);
    func_08A51EB8(self);
}
