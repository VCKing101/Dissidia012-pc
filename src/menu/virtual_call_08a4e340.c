typedef struct VirtualEntry {
    short delta;
    void (*fn)(char *self);
} VirtualEntry;

typedef struct VirtualObj {
    char pad_00[0x24];
    char *field_24;
} VirtualObj;

typedef struct StepCtx {
    char pad_000[0xC];
    int field_0C;
    char pad_10[0x6FE4 - 0x10];
    VirtualObj *field_6FE4;
} StepCtx;

typedef struct StepOwner {
    char pad_00[0x5C];
    StepCtx *field_5C;
} StepOwner;

extern void func_08A59F18(VirtualObj *obj, int value, int ctx_value);

void func_08A4E340(StepOwner *self, int value)
{
    VirtualObj *obj = self->field_5C->field_6FE4;
    VirtualEntry *entry = (VirtualEntry *)(obj->field_24 + 0x28);
    entry->fn((char *)obj + entry->delta);
    func_08A59F18(self->field_5C->field_6FE4, value, self->field_5C->field_0C);
}
