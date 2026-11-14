typedef struct VirtualEntry {
    short delta;
    void (*fn)(char *self);
} VirtualEntry;

typedef struct VirtualObj {
    char pad_00[0x24];
    char *field_24;
    char pad_28[0x44 - 0x28];
    int field_44;
    int field_48;
} VirtualObj;

extern int func_088DAA68(int handle);

void func_08A698F4(VirtualObj *self)
{
    VirtualEntry *entry = (VirtualEntry *)(self->field_24 + 0x48);
    entry->fn((char *)self + entry->delta);
    self->field_48 = func_088DAA68(self->field_44);
}
