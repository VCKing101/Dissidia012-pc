/* Clears the id and resets the three 0x1E-byte entries. */
struct Entry1E {
    char data[0x1E];
};

struct EntryTriple {
    int id;
    Entry1E entries[3];
};

extern "C" {
extern void func_088B7D04(Entry1E *entry);

void func_08902A10(EntryTriple *t)
{
    t->id = -1;
    for (int i = 0; i < 3; i++) {
        func_088B7D04(&t->entries[i]);
    }
}
}
