/* Fills the first free of five 0xD8-byte slots; returns 1 when one was free. */
typedef struct ClaimSlot {
    int used;
    char pad_04[0xD8 - 4];
} ClaimSlot;

extern void func_088F3550(ClaimSlot *slots, int index, int arg);

int func_088F3704(ClaimSlot *slots, int arg)
{
    int result = 0;
    int i;

    for (i = 0; i < 5; i++) {
        if (slots[i].used == 0) {
            func_088F3550(slots, i, arg);
            result = 1;
            break;
        }
    }
    return result;
}
