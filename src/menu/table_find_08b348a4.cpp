/* True when `key` is found in the object's table (false without a table). */
struct Table08B348A4 {
    char pad_0[4];
    int field_4;
    char field_8[8];
};

struct Object08B348A4 {
    char pad_000[0x178];
    Table08B348A4 *field_178;
};

extern "C" {
extern int D_08B343D8;
int func_08934944(void *key, char *entries, int count, int size, void *compare);
}

struct Check08B348A4 {
    static bool present(Table08B348A4 *table) { return table != 0; }
};

extern "C" {
unsigned char func_08B348A4(Object08B348A4 *self, void *key)
{
    Table08B348A4 *table = self->field_178;
    if (!Check08B348A4::present(table)) {
        return 0;
    }
    return func_08934944(key, table->field_8, table->field_4, 8, &D_08B343D8) != 0;
}
}
