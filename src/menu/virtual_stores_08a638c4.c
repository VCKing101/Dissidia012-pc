typedef struct VirtualEntry {
    short delta;
    void (*fn)(char *self);
} VirtualEntry;

typedef struct Inner {
    char pad_00[0x10];
    char *field_10;
} Inner;

typedef struct ClearObj {
    char pad_00[3];
    char field_03;
    char field_04;
    char pad_05[3];
    int field_08;
    char *field_0C;
} ClearObj;

void func_08A638C4(ClearObj *self)
{
    VirtualEntry *entry = (VirtualEntry *)(self->field_0C + 0x10);
    entry->fn((char *)self + entry->delta);
    self->field_08 = 0;
    self->field_03 = 0;
    self->field_04 = 0;
}
