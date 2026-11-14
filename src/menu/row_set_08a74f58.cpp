/* Fills the three rows with the given ids. */
struct SetRow {
    char data[0x14];
};

extern "C" void func_08A74BF8(SetRow *row, void *owner, int id);

extern "C" {
void func_08A74F58(SetRow *rows, void *owner, short *ids)
{
    for (int i = 0; i < 3; i++) {
        SetRow *row = &rows[i];
        func_08A74BF8(row, owner, ids[i]);
    }
}
}
