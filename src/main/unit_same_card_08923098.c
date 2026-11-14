typedef struct UnitBody { char pad_00[0xB4]; void *card; } UnitBody;
typedef struct Unit { char pad_00[0x20]; UnitBody *body; } Unit;
extern int func_08925378(void *card);
int func_08923098(Unit *unit, void *card)
{
    void *own = unit->body->card;
    if (own != 0) {
        int id = func_08925378(own);
        if (id == func_08925378(card)) {
            return 1;
        } else {
            return 0;
        }
    }
    return 0;
}
