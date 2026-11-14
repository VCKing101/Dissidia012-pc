typedef struct CardSlot { short state; short count; } CardSlot;
extern int func_0892346C(CardSlot *slot);
extern void func_08924CE4(CardSlot *slot, int state, int flag);
int func_089234C0(CardSlot *slot)
{
    if (func_0892346C(slot) != 0) {
        int result = 1;
        func_08924CE4(slot, 3, 1);
        return result;
    } else {
        return 0;
    }
}
