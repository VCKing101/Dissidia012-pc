/* Adds up the twelve amounts of item `id` for the current mode, or 0 when the
   id is negative or has no record. */
struct Record08ACFAD0 {
    char pad_0[0xA];
    short field_A;
};

struct State08ACFAD0 {
    char pad_0000[0x1214];
    int field_1214;
};

struct Screen08ACFAD0 {
    char pad_0000[0xDB30];
    struct State08ACFAD0 *field_DB30;
};

extern struct Record08ACFAD0 *func_088B1D9C(int id);
extern void func_08AD75F0(struct Screen08ACFAD0 *self, int slot, int id, short *amount);

short func_08ACFAD0(struct Screen08ACFAD0 *self, short id)
{
    short total;
    short amount[1];
    struct Record08ACFAD0 *record;
    int base;
    int i;
    if (id < 0) {
        return 0;
    }
    total = 0;
    amount[0] = 0;
    record = func_088B1D9C(id);
    if (record != 0) {
        if (record->field_A != -1) {
            base = self->field_DB30->field_1214 * 12;
            for (i = 0; i < 12; i++) {
                func_08AD75F0(self, base + i, id, amount);
                total = total + amount[0];
            }
        }
    }
    return total;
}
