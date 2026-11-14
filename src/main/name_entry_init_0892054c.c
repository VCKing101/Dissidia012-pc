/* Initialises a name entry and gives it an id. */
typedef struct NameEntry { int field_00; short id; } NameEntry;
extern void func_088BB760(short *id, int value);
extern void func_089205C0(NameEntry *entry);
NameEntry *func_0892054C(NameEntry *entry, int id)
{
    func_088BB760(&entry->id, -1);
    func_089205C0(entry);
    entry->id = id;
    return entry;
}
