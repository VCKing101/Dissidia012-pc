typedef struct FighterMoveSet {
    char pad0000[0x180];
    short mode;             /* 0x180 */
    char pad0182[0x212];
    void *motion;           /* 0x394 */
    void *owner;            /* 0x398 */
    char pad039C[0x4A4];
    char anim[0x8E0];       /* 0x840 */
    char track[4];          /* 0x1120 */
} FighterMoveSet;

extern void func_08830D6C(void *anim, void *motion);
extern void func_08A62390(void *track, void *motion);
extern void func_0880EA20(void *owner, int value, FighterMoveSet *fighter);

void func_08B30814(FighterMoveSet *fighter, int value)
{
    if (fighter->mode == 1 || fighter->mode == 2) {
        func_08830D6C(fighter->anim, fighter->motion);
    }
    func_08A62390(fighter->track, fighter->motion);
    func_0880EA20(fighter->owner, value, fighter);
}
