/* Slot setter: stores a value at 0x20 once the slot reports ready. */
typedef struct SlotRecord {
    char pad_00[0x20];
    int value;
} SlotRecord;

extern int func_088D723C(SlotRecord *slot);

int func_088D73F4(SlotRecord *slot, int value)
{
    if (func_088D723C(slot) == 0) {
        return -1;
    }
    slot->value = value;
    return 0;
}
