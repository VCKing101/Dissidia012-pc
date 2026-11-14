typedef struct FighterLink {
    char pad000[0x5C4];
    void *link;             /* 0x5C4 */
} FighterLink;

extern int func_08A640AC(void *link);

int func_08B2F3B0(FighterLink *fighter)
{
    void *link = fighter->link;
    int state;

    if (link == 0) {
        return 0;
    }
    state = func_08A640AC(link);
    return state == 0 || state == 3 || state == 4 || state == 5;
}
