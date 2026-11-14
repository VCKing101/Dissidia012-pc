extern "C" {
typedef struct CardRule { char pad_00[0x12]; short kind; } CardRule;
typedef struct CardSlot { short state; short count; char pad_04[0x1C]; CardRule *rule; } CardSlot;
extern void func_0892397C(CardSlot *slot, int state);
bool func_08923280(CardSlot *slot)
{
    switch (slot->rule->kind) {
    case 3:
    case 5:
        if (slot->count == 1) {
            func_0892397C(slot, 2);
            return true;
        }
    }
    return false;
}
}
