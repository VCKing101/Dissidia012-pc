extern "C" {
int func_0889710C(const void *a, const void *b);
void *func_08934944(const void *key, const void *base, int count, int size, int (*compare)(const void *, const void *));
}

struct Table08897138 {
    int field_00;
    int count;
    unsigned char entries[4];

    static void *find(Table08897138 *table, const void *key) {
        return func_08934944(key, table->entries, table->count, 0x5C, func_0889710C);
    }
};

struct Owner08897138 {
    Table08897138 *table;
};

extern "C" {
void *func_08897138(Owner08897138 *self, const void *key) {
    return Table08897138::find(self->table, key);
}
}
