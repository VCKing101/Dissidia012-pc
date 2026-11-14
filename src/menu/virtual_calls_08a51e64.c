typedef struct VirtualEntry {
    short delta;
    void (*fn)(char *self);
} VirtualEntry;

typedef struct VirtualObj {
    char pad_00[0x24];
    char *field_24;
} VirtualObj;

extern void func_08A521FC();

void func_08A51E64(VirtualObj *self)
{
    VirtualEntry *entry;
    func_08A521FC();
    entry = (VirtualEntry *)(self->field_24 + 0x10);
    entry->fn((char *)self + entry->delta);
    entry = (VirtualEntry *)(self->field_24 + 0x38);
    entry->fn((char *)self + entry->delta);
}
