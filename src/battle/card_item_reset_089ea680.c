/* Resets a card item: links its sixteen record slots into a ring, clears
 * the records, and clears every state field. */
typedef struct CardRecord {
    unsigned char data[0x78];
} CardRecord;

typedef struct CardSlot {
    CardRecord *record;
    struct CardSlot *next;
} CardSlot;

typedef struct CardItemState {
    unsigned char open;
    unsigned char pad_01[3];
    int index;
    CardSlot slots[16];
    CardRecord records[16];
    CardSlot *free_read;
    CardSlot *free_write;
    void *head;
    void *tail;
    int count;
    unsigned char pad_81c[0x10];
    unsigned char flags[2];
    unsigned char shown;
    unsigned char pad_82f;
    int field_830;
    float field_834;
    int field_838;
    int field_83c;
    int field_840;
    int field_844;
} CardItemState;

extern void func_08935828(void *dst, int value, unsigned int size);

void func_089EA680(CardItemState *item)
{
    item->open = 0;
    item->index = -1;
    {
        int i;
        for (i = 0; i < 16; i++) {
            item->slots[i].record = &item->records[i];
            item->slots[i].next = &item->slots[i + 1];
        }
    }
    func_08935828(item->records, 0, sizeof(item->records));
    item->slots[15].next = item->slots;
    item->free_read = item->slots;
    item->free_write = item->slots;
    item->tail = 0;
    item->head = 0;
    item->count = 0;
    {
        int request;
        for (request = 0; request < 2; request++) {
            item->flags[request] = 0;
        }
    }
    item->shown = 0;
    item->field_830 = 0;
    item->field_834 = 0.0f;
    item->field_838 = 0;
    item->field_83c = -1;
    item->field_840 = 0;
    item->field_844 = 0;
}
