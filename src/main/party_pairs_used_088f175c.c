/* Counts the used entries (id != -1) of the first or second pair list. */
typedef struct PartyEntry {
    short id;
    short value;
} PartyEntry;

typedef struct PartyLists {
    char pad_00[0xC];
    PartyEntry first[2];
    PartyEntry second[2];
} PartyLists;

typedef struct SaveData {
    char pad_00000[0x1DEE0];
    PartyLists party;
} SaveData;

extern SaveData *D_089D712C;

int func_088F175C(int which)
{
    PartyLists *lists = &D_089D712C->party;
    PartyEntry *list;
    int count;
    int i;

    if (which == 0) {
        list = lists->first;
    } else {
        list = lists->second;
    }
    count = 0;
    for (i = 0; i < 2; i++) {
        if (list[i].id != -1) {
            count++;
        }
    }
    return count;
}
