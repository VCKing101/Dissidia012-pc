typedef struct VirtualEntry {
    short delta;
    void (*fn)(char *self, int a, int b, int c);
} VirtualEntry;

typedef struct VirtualHost {
    char pad_00[116];
    char *field_74;
} VirtualHost;

extern void func_08A51E64(VirtualHost *self, int a, int b, int c);

void func_08A5CCE8(VirtualHost *self, int a, int b, int c)
{
    VirtualEntry *entry;
    char *sub;
    func_08A51E64(self, a, b, c);
    entry = (VirtualEntry *)(self->field_74 + 0x18);
    sub = (char *)self + 0x50;
    entry->fn(sub + entry->delta, a, b, c);
}
