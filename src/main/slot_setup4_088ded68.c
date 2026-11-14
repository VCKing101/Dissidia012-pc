typedef struct Slot4B {
    unsigned char active;
    char pad_01[7];
    void *first;
    void *second;
    void *third;
    unsigned int index;
} Slot4B;

extern void func_088DED28(Slot4B *slot);

int func_088DED68(Slot4B *slot, void *first, void *second, void *third, unsigned int index)
{
    func_088DED28(slot);
    if (first == 0 || second == 0 || third == 0 || index >= 2) {
        return -1;
    }
    slot->first = first;
    slot->second = second;
    slot->third = third;
    slot->index = index;
    slot->active = 1;
    return 0;
}
