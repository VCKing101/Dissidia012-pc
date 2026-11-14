typedef struct VirtualEntry {
    short delta;
    void (*fn)(char *self);
} VirtualEntry;

typedef struct Inner {
    char pad_00[0x24];
    char *field_24;
} Inner;

typedef struct Holder {
    char pad_00[4];
    Inner *field_04;
} Holder;

void func_08A51FD4(Holder *self)
{
    Inner *inner;
    VirtualEntry *entry;
    if (self == 0) {
        return;
    }
    inner = self->field_04;
    if (inner == 0) {
        return;
    }
    entry = (VirtualEntry *)(inner->field_24 + 0x30);
    entry->fn((char *)inner + entry->delta);
}
