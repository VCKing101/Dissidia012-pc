/* Resets the slot and stores three non-null pointers and an index below 7. */
typedef struct Slot4 {
    unsigned char active;
    char pad_01[0x2C - 1];
    void *first;
    void *second;
    void *third;
    unsigned int index;
} Slot4;

extern void func_088D8A54(Slot4 *slot);

int func_088D8B58(Slot4 *slot, void *first, void *second, void *third, unsigned int index)
{
    func_088D8A54(slot);
    if (first == 0 || second == 0 || third == 0 || index >= 7) {
        return -1;
    }
    slot->first = first;
    slot->second = second;
    slot->third = third;
    slot->index = index;
    slot->active = 1;
    return 0;
}
