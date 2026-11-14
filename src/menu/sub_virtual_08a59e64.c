typedef struct VirtualEntry {
    short delta;
    void (*fn)(char *self, int a, int b, int c);
} VirtualEntry;

typedef struct VirtualHost {
    char pad_00[6568];
    char *field_19A8;
} VirtualHost;

extern void func_08A51E64(VirtualHost *self, int a, int b, int c);

void func_08A59E64(VirtualHost *self, int a, int b, int c)
{
    VirtualEntry *entry;
    char *sub;
    func_08A51E64(self, a, b, c);
    entry = (VirtualEntry *)(self->field_19A8 + 0x18);
    sub = (char *)self + 0x1984;
    entry->fn(sub + entry->delta, a, b, c);
}
