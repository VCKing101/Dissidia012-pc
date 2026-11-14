/* Constructs a kind-9 unit, sets its flags and appends it to the scene's unit list. */
struct Unit08814994;

struct UnitLink08814994 {
    Unit08814994 *next;
};

struct Unit08814994 {
    char pad_000[0x388];
    unsigned long long flags;
    char pad_390[0x1B4];
    void *vtable;
    char pad_548[8];
    UnitLink08814994 link;

    static UnitLink08814994 *linkOf(Unit08814994 *u) { return &u->link; }
};

struct UnitList08814994 {
    Unit08814994 *head;
    Unit08814994 *tail;

    static void append(UnitList08814994 *list, Unit08814994 *unit)
    {
        if (list->tail == 0) {
            list->tail = unit;
            list->head = unit;
            unit->link.next = 0;
        } else {
            Unit08814994::linkOf(list->tail)->next = unit;
            unit->link.next = 0;
            list->tail = unit;
        }
    }
};

struct Scene08814994 {
    char pad_00[0x34];
    UnitList08814994 units;
};

extern "C" {

extern char D_089C7030[];
extern unsigned long long D_089C7240;
extern Scene08814994 *D_089D4958;

void func_0883975C(Unit08814994 *unit, int kind, int arg);
void func_088371C4(Unit08814994 *unit);
void func_08834BB8(Unit08814994 *unit, int a, int b);

Unit08814994 *func_08814994(Unit08814994 *unit, int arg)
{
    func_0883975C(unit, 9, arg);
    unit->vtable = D_089C7030;
    func_088371C4(unit);
    unit->flags |= D_089C7240;
    func_08834BB8(unit, 0, 0);
    UnitList08814994::append(&D_089D4958->units, unit);
    return unit;
}

}
