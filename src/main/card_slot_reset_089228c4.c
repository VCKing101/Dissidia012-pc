/* Resets a card slot record. */
typedef struct CardSlot {
    short state;                /* 0x00 */
    short count;                /* 0x02 */
    short level;                /* 0x04 */
    short id;                   /* 0x06 */
    int values[5];              /* 0x08 */
    int field_1C;
    char pad_20[4];
} CardSlot;

void func_089228C4(CardSlot *slot)
{
    int i;
    slot->state = 0;
    slot->count = 1;
    slot->level = 1;
    slot->id = -1;
    for (i = 0; i < 5; i++) {
        slot->values[i] = 0;
    }
    slot->field_1C = 0;
}
