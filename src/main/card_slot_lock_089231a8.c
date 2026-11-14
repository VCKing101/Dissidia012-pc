typedef struct CardSlot { short state; short count; } CardSlot;
extern void func_0892397C(CardSlot *slot, int state);
void func_089231A8(CardSlot *slot)
{
    if (slot->count > 0 && slot->count < 3) {
        func_0892397C(slot, 3);
    }
}
