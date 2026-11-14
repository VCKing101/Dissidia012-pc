/* Finds the registered 0x24-byte entry whose record id matches; 0 when none. */
struct EntryInfo088B36FC {
    char pad_00[8];
    int count;
};

struct Record088B36FC {
    short id;
};

struct Entry088B36FC {
    char pad_00[0xC];
    Record088B36FC *record;
    char pad_10[0x24 - 0x10];
};

extern "C" {

extern EntryInfo088B36FC *D_089D72E8;
extern Entry088B36FC *D_089D72EC;

Entry088B36FC *func_088B36FC(int id)
{
    Entry088B36FC *result = 0;
    if (D_089D72E8 == 0) {
        return 0;
    }
    for (int i = 0; i < D_089D72E8->count; i++) {
        Entry088B36FC *entry = &D_089D72EC[i];
        if (entry->record->id == id) {
            result = entry;
            break;
        }
    }
    return result;
}

}
