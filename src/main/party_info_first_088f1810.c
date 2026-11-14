/* Fills `info` (0xFF when empty) from the first selected character's record. */
typedef struct CharRecord {
    char pad_00[2];
    unsigned short level;
    unsigned short id;
    unsigned short costume;
    char pad_08[0xC - 8];
    unsigned short weapon;
    char pad_0E[0x14 - 0xE];
    unsigned short extra[4];
    unsigned char rank;
} CharRecord;

typedef struct CharInfo {
    unsigned short id;
    unsigned short level;
    unsigned short weapon;
    unsigned short costume;
    unsigned short extra[4];
    unsigned char rank;
} CharInfo;

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
extern void func_08935828(void *dst, int value, int size);

void func_088F1810(CharInfo *info)
{
    PartySel *party = &D_089D712C->party;
    CharRecord *rec = func_088C4F74(party->first);

    func_08935828(info, 0xFF, 0x12);
    if (rec != 0) {
        info->id = rec->id;
        info->level = rec->level;
        info->costume = rec->costume;
        info->weapon = rec->weapon;
        info->rank = rec->rank;
        info->extra[0] = rec->extra[0];
        info->extra[1] = rec->extra[1];
        info->extra[2] = rec->extra[2];
        info->extra[3] = rec->extra[3];
    }
}
