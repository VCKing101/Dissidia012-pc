/* Closes an open card slot (count 1) when its rule allows it. */
extern "C" {

typedef struct CardSlot {
    short state;                /* 0x00 */
    short count;                /* 0x02 */
} CardSlot;

extern int func_0892346C(CardSlot *slot);
extern void func_08924CE4(CardSlot *slot, int state, int flag);

bool func_08923514(CardSlot *slot)
{
    if (func_0892346C(slot) != 0 && slot->count == 1) {
        func_08924CE4(slot, 2, 0);
        return true;
    } else {
        return false;
    }
}

}
