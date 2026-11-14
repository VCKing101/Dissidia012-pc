typedef struct CardRule { char pad_00[0x12]; short kind; } CardRule;
typedef struct CardSlot { short state; short count; char pad_04[0x1C]; CardRule *rule; } CardSlot;
int func_0892346C(CardSlot *slot)
{
    int result = 0;
    switch (slot->rule->kind) {
    case 3:
    case 5:
        if (slot->count > 0 && slot->count < 3) {
            result = 1;
        }
        break;
    }
    return result;
}
