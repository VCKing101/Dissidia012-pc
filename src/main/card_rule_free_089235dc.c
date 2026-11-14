typedef struct CardSlot { short state; short count; char pad_04[0x1C]; void *rule; } CardSlot;
extern int func_08935970(void *rule);
int func_089235DC(CardSlot *slot)
{
    if (func_08935970(slot->rule) == 0) {
        return 1;
    } else {
        return 0;
    }
}
