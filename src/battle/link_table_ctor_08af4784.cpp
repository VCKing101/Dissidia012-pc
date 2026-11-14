typedef struct LinkEntry {
    int first;
    int second;
    int value;
} LinkEntry;

typedef struct LinkTable {
    char base[0x10];
    const void *vtable;
    int field_14;
    LinkEntry entries[10];
    int field_90;
} LinkTable;

extern "C" {

extern void func_08AF3E08(LinkTable *self);
extern const unsigned char D_08AFB5E8[];

LinkTable *func_08AF4784(LinkTable *self)
{
    func_08AF3E08(self);
    self->vtable = D_08AFB5E8;
    self->field_14 = 0;
    self->field_90 = 0;
    for (int i = 0; i < 10; i++) {
        self->entries[i].first = -1;
        self->entries[i].second = -1;
        self->entries[i].value = 0;
    }
    return self;
}

}
