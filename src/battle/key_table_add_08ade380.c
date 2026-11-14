typedef struct EntryKey {
    int id;
} EntryKey;

typedef struct KeyTable KeyTable;

extern int func_08ADE648(KeyTable *self, int id);
extern void func_08ADE6A4(KeyTable *self, EntryKey key);

/* Add id unless the table already has it. */
void func_08ADE380(KeyTable *self, int id)
{
    if (func_08ADE648(self, id) == -1) {
        EntryKey key;
        key.id = id;
        func_08ADE6A4(self, key);
    }
}
