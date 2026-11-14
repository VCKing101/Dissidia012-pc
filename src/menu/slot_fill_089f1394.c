/* Screen0B6A0: fills a result slot from a pair of ids: the first when it
   names a known record (its kind goes to +0x01), else a kind picked by
   func_089F0DA4 from the second. */
typedef struct SlotKind29 {
    char pad_00[0x2];
    unsigned char kind;
} SlotKind29;

typedef struct ResultSlot29 {
    char pad_00[0x1];
    unsigned char kind;
    char pad_02[0xA];
    short id;
} ResultSlot29;

extern SlotKind29 *func_088BBD8C(int id);
extern unsigned char func_089F0DA4(void *self, int id, int arg1, int arg2);

void func_089F1394(void *self, short *ids, ResultSlot29 *slot, int arg1, int arg2)
{
    slot->id = ids[0];
    if (slot->id != -1) {
        SlotKind29 *record = func_088BBD8C(slot->id);
        if (record != 0) {
            slot->kind = record->kind;
        } else {
            slot->id = -1;
        }
    }
    if (slot->id == -1) {
        slot->kind = func_089F0DA4(self, ids[1], arg1, arg2);
    }
}
