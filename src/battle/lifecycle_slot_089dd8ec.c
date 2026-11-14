/* Turns one of the three root slots off or on and refreshes the root. */
typedef struct BattleRootSlots {
    unsigned char field_00;
    unsigned char padding_01[11];
    unsigned char field_0C[3];
} BattleRootSlots;

extern int func_089DDECC(void *);

int func_089DD8EC(BattleRootSlots *object, unsigned int slot)
{
    if (!object->field_00) return -1;
    if (slot >= 3) return -2;
    object->field_0C[slot] = 0;
    if (func_089DDECC(object)) return -3;
    return 0;
}

int func_089DD954(BattleRootSlots *object, unsigned int slot)
{
    if (!object->field_00) return -1;
    if (slot >= 3) return -2;
    object->field_0C[slot] = 1;
    if (func_089DDECC(object)) return -3;
    return 0;
}
