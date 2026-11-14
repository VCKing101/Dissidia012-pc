typedef struct PairEntry {
    int first;
    char pad_04[0x8];
    int second;
    char pad_10[0x4];
} PairEntry;

typedef struct PairList {
    char pad_00[0x8];
    PairEntry *entries;
    char pad_0C[0x28];
    int field_34;
} PairList;

typedef struct ListInfo {
    char pad_00[0x4C];
    int count;
} ListInfo;

extern "C" int D_08AAAD80;
extern "C" ListInfo *func_08AA79C4(int id);

extern "C" void func_08AA616C(PairList *self)
{
    int count = func_08AA79C4(D_08AAAD80)->count;

    for (int i = 0; i < count; i++) {
        self->entries[i].first = 0;
        self->entries[i].second = 0;
    }
    self->field_34 = 0;
}
