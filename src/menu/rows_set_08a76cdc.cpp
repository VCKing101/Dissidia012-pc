/* Fills the first count rows from the last to the first; only row 0 is marked as the head. */
typedef struct ListRow {
    char data[0x20];
} ListRow;

typedef struct RowList {
    ListRow field_00[3];
    int field_60;
} RowList;

extern "C" void func_08A76938(ListRow *row, void *source);
extern "C" void func_08A76BF8(ListRow *row, int head);

extern "C" void func_08A76CDC(RowList *self, void *source, int count)
{
    self->field_60 = count;
    for (int i = count - 1; i >= 0; i--) {
        func_08A76938(&self->field_00[i], source);
        if (i == 0) {
            func_08A76BF8(&self->field_00[i], 1);
        } else {
            func_08A76BF8(&self->field_00[i], 0);
        }
    }
}
