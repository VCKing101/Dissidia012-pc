typedef struct FighterStun {
    char pad0000[0x10CE];
    short stun;             /* 0x10CE */
    int stunned;            /* 0x10D0 */
} FighterStun;

void func_08B33914(FighterStun *fighter)
{
    if (fighter->stun > 0) {
        fighter->stun--;
        fighter->stunned = 1;
    }
}
