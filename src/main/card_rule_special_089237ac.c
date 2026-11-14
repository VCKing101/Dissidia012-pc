typedef struct CardRule { char pad_00[0x16]; short id; } CardRule;
typedef struct CardSlot { short state; short count; char pad_04[0x1C]; CardRule *rule; } CardSlot;
int func_089237AC(CardSlot *slot)
{
    switch (slot->rule->id) {
    case 0xCF:
    case 0x134:
    case 0x135:
        return 1;
    }
    return 0;
}
