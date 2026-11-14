/* Finds the registered 0x1C-byte entry with the given id; 0 when none. */
struct EntryInfo088B37C0 {
    char pad_00[8];
    int count;
};

struct Entry088B37C0 {
    short id;
    char pad_02[0x1C - 2];
};

extern "C" {

extern EntryInfo088B37C0 *D_089D72F0;
extern Entry088B37C0 *D_089D72F4;

Entry088B37C0 *func_088B37C0(int id)
{
    Entry088B37C0 *result = 0;
    if (D_089D72F0 == 0) {
        return 0;
    }
    for (int i = 0; i < D_089D72F0->count; i++) {
        Entry088B37C0 *entry = &D_089D72F4[i];
        if (entry->id == id) {
            result = entry;
            break;
        }
    }
    return result;
}

}
