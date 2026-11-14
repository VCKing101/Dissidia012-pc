typedef struct FighterLink {
    char pad000[0x5C4];
    void *link;             /* 0x5C4 */
} FighterLink;

extern int func_08A640AC(void *link);

unsigned char func_08B2F408(FighterLink *fighter)
{
    int result = 0;

    if (fighter->link != 0 && func_08A640AC(fighter->link) == 2) {
        result = 1;
    }
    return result;
}
