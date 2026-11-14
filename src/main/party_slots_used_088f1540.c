/* Counts the used extra slots (0x16/0x18/0x1A not 0xFFFF) of the selected character. */
typedef struct CharRecord {
    char pad_00[0x16];
    unsigned short extra[3];
} CharRecord;

typedef struct PartySel {
    short first;
    short second;
} PartySel;

typedef struct SaveData {
    char pad_00000[0x1DEE0];
    PartySel party;
} SaveData;

extern SaveData *D_089D712C;
extern CharRecord *func_088C4F74(int id);

int func_088F1540(void)
{
    PartySel *party = &D_089D712C->party;
    CharRecord *rec = func_088C4F74(party->first);
    int count = 0;

    if (rec->extra[0] != 0xFFFF) {
        count = 1;
    }
    if (rec->extra[1] != 0xFFFF) {
        count++;
    }
    if (rec->extra[2] != 0xFFFF) {
        count++;
    }
    return count;
}
