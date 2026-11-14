typedef struct VirtualEntry {
    short delta;
    void (*fn)(char *self);
} VirtualEntry;

typedef struct VirtualObj {
    char pad_00[0x24];
    char *field_24;
} VirtualObj;

extern void func_08A60408(char *target);

void func_08A5E0E4(VirtualObj *self)
{
    VirtualEntry *entry;
    func_08A60408((char *)self + 0xA0);
    entry = (VirtualEntry *)(self->field_24 + 0x50);
    entry->fn((char *)self + entry->delta);
    entry = (VirtualEntry *)(self->field_24 + 0x48);
    entry->fn((char *)self + entry->delta);
}
