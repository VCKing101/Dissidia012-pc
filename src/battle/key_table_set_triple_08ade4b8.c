typedef struct KeyTable KeyTable;

typedef struct TripleEntry {
    int a;
    int b;
    int c;
} TripleEntry;

extern void func_08ADEAA8(KeyTable *self, TripleEntry *entry);

void func_08ADE4B8(KeyTable *self, int a, int b, int c)
{
    TripleEntry entry;
    entry.a = a;
    entry.c = c;
    entry.b = b;
    func_08ADEAA8(self, &entry);
}

