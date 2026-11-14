typedef struct KeyTable KeyTable;

typedef struct PairEntry {
    int first;
    int second;
} PairEntry;

extern int func_08ADE648(KeyTable *self, int id);
extern void func_08ADE9A4(KeyTable *self, PairEntry *entry);

/* Add the pair unless the table already has id. */
void func_08ADE458(KeyTable *self, int first, int second, int id)
{
    if (func_08ADE648(self, id) == -1) {
        PairEntry entry;
        entry.first = first;
        entry.second = second;
        func_08ADE9A4(self, &entry);
    }
}
