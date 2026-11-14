typedef struct VirtualEntry {
    short delta;
    void (*fn)(char *self);
} VirtualEntry;

typedef struct VirtualHost {
    char pad_00[6568];
    char *field_19A8;
} VirtualHost;

extern void func_08A51EB8(VirtualHost *self);

void func_08A59ED4(VirtualHost *self)
{
    char *sub = (char *)self + 0x1984;
    VirtualEntry *entry = (VirtualEntry *)(self->field_19A8 + 0x20);
    entry->fn(sub + entry->delta);
    func_08A51EB8(self);
}
