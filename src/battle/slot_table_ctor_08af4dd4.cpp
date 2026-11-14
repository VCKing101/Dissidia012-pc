typedef struct SlotEntry {
    int id;
    int value;
    int extra;
} SlotEntry;

typedef struct SlotTable {
    char base[0x10];
    const void *vtable;
    int field_14;
    SlotEntry entries[32];
    int field_198;
} SlotTable;

extern "C" {

extern void func_08AF3E08(SlotTable *self);
extern const unsigned char D_08AFB630[];

SlotTable *func_08AF4DD4(SlotTable *self)
{
    func_08AF3E08(self);
    self->vtable = D_08AFB630;
    self->field_14 = 0;
    self->field_198 = 0;
    for (int i = 0; i < 32; i++) {
        self->entries[i].id = -1;
        self->entries[i].value = 0;
        self->entries[i].extra = 0;
    }
    return self;
}

}
