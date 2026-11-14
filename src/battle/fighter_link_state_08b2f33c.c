typedef struct FighterLink {
    char pad000[0x5C4];
    void *link;             /* 0x5C4 */
} FighterLink;

extern int func_08A640AC(void *link);

int func_08B2F33C(FighterLink *fighter)
{
    if (fighter->link == 0) {
        return -1;
    }
    return func_08A640AC(fighter->link);
}
