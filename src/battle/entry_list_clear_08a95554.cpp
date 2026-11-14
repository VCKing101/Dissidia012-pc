typedef struct ListEntry {
    int value;
    char pad_04[0x8];
} ListEntry;

typedef struct EntryList {
    char pad_00[0x4];
    ListEntry *entries;
    char pad_08[0x4];
    int field_0C;
} EntryList;

typedef struct ListInfo {
    char pad_00[0x38];
    int count;
} ListInfo;

extern "C" int D_08AAAD80;
extern "C" ListInfo *func_08AA79C4(int id);

extern "C" void func_08A95554(EntryList *self)
{
    int count = func_08AA79C4(D_08AAAD80)->count;

    for (int i = 0; i < count; i++) {
        self->entries[i].value = 0;
    }
    self->field_0C = 0;
}
