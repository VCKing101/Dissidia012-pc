/* Clears an entry: two ids, four values and the trailing word. */
struct Entry088BD398 {
    short id;
    short kind;
    short values[4];
    int extra;
};

extern "C" {

Entry088BD398 *func_088BD398(Entry088BD398 *entry)
{
    entry->id = 0;
    entry->kind = 0;
    entry->extra = 0;
    for (int i = 0; i < 4; i++) {
        entry->values[i] = 0;
    }
    return entry;
}

}
