/* While flag 0x80000 is set, moves the cursor onto the current slot's part. */
struct PoseMatrix {
    float m[16];
};

struct PoseTable {
    char pad_000[0x51C];
    void *field_51C[(0x6AC - 0x51C) / 4];
    void *field_6AC;
    char pad_6B0[0x2724 - 0x6B0];
    unsigned int field_2724;

    /* Ten pointers per slot from +0x51C. */
    static void *part(PoseTable *table, int index) { return table->field_51C[index]; }
};

struct PoseOwner {
    char pad_000[0x14C];
    PoseTable *field_14C;
};

extern "C" int func_089F2FF0(PoseOwner *self);
extern "C" void func_088DA0A8(void *part, int node, PoseMatrix *matrix, int *extra);
extern "C" void func_088D9B84(void *cursor, PoseMatrix *matrix);
extern "C" void func_088D9AF4(void *cursor, int *extra);

extern "C" {
void func_089F6E58(PoseOwner *self)
{
    if ((self->field_14C->field_2724 & 0x80000) != 0) {
        PoseMatrix matrix;
        int extra;
        int slot = func_089F2FF0(self);
        func_088DA0A8(PoseTable::part(self->field_14C, slot * 10), 0, &matrix, &extra);
        func_088D9B84(self->field_14C->field_6AC, &matrix);
        func_088D9AF4(self->field_14C->field_6AC, &extra);
    }
}
}
