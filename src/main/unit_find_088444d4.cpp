/* Walks the unit list and returns the first unit whose definition id matches. */
struct UnitDef {
    short id;
};

struct ListUnit {
    UnitDef *def;
    char pad_04[0xE4];
    ListUnit *next;
};

struct UnitRegistry {
    int field_00;
    int field_04;
    ListUnit *head;

    static ListUnit *next(UnitRegistry *r, ListUnit *u) { return u == 0 ? r->head : u->next; }
};

extern "C" {

ListUnit *func_088444D4(UnitRegistry *r, int id)
{
    ListUnit *u = 0;

    while ((u = UnitRegistry::next(r, u)) != 0) {
        if (u->def->id == id) {
            return u;
        }
    }
    return 0;
}

}
