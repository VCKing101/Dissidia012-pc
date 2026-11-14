/* Resets the head entry (enabled) and the five following entries. */
typedef struct SlotEntry {
    short id;
    unsigned char enabled;
    unsigned char state;
} SlotEntry;

typedef struct SlotList {
    SlotEntry head;
    SlotEntry rest[5];
} SlotList;

void func_088F93E8(SlotList *list)
{
    int i;

    list->head.id = -1;
    list->head.state = 0;
    list->head.enabled = 1;
    for (i = 0; i < 5; i++) {
        SlotEntry *entry = &list->rest[i];

        entry->id = -1;
        entry->enabled = 0;
        entry->state = 0;
    }
}
