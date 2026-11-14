typedef struct FighterLink {
    char pad000[0x5C4];
    void *link;             /* 0x5C4 */
} FighterLink;

extern void func_08A64134(void *link, int a);
extern void func_08A64100(void *link, int b);

int func_08B2F274(FighterLink *fighter, int a, int b)
{
    if (a == 0) {
        return -1;
    }
    if (fighter->link == 0) {
        return -2;
    }
    func_08A64134(fighter->link, a);
    func_08A64100(fighter->link, b);
    return 0;
}
