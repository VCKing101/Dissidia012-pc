/* The save manager looks for a name slot (of 100, 20 bytes each) equal to one of the save object's count (+0xE6C) 72-byte records at +0xEA8; returns the slot index or -1. */
typedef struct SaveSlot { char name[0x14]; } SaveSlot;
typedef struct SaveRecord { char data[0x48]; } SaveRecord;
typedef struct SaveObj {
    SaveSlot slots[100];
    char pad[0xE6C - 100 * 0x14];
    unsigned int count;
    char pad2[0xEA8 - 0xE70];
    SaveRecord records[1];
} SaveObj;
typedef struct SaveManager {
    int unk_00;
    unsigned int flags;
    int *buffer;
    SaveObj *save;
} SaveManager;

int func_08935B50(void *a, void *b, int n);

int func_088AF7B0(SaveManager *manager)
{
    unsigned int i;
    int j;
    for (i = 0; i < manager->save->count; i++) {
        for (j = 0; j < 100; j++) {
            if (func_08935B50(&manager->save->slots[j], &manager->save->records[i], 0x14) == 0) {
                return j;
            }
        }
    }
    return -1;
}
