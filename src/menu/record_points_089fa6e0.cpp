#include "menu/agent29_types.h"

/* Screen0B6A0: works out the points of record `id`: sets the record's
   character level from the save, keeps the record's id, level and
   rank (+0x32E4..), builds the record's unit and its stats and gives the
   points (at most 999999); with `apply` also the unit's bonus (+0x32F4).
   The unit is a C++ object; its constructor and destructor (called with
   flag 2) are called by hand here. */
typedef struct Unit29 {
    char data[0x5E4];
} Unit29;

typedef struct Stats29 {
    char pad_00[0x2C];
    int points; /* +0x2C */
} Stats29;

typedef struct UnitRef29 {
    char data[0xC];
} UnitRef29;

typedef struct Fighter29 {
    char pad_00[0x1];
    unsigned char kind;
} Fighter29;

extern "C" {
Fighter29 *func_088B70C8(int id);
void *func_088C09FC(int kind);
void func_088C07B4(void *fighter, int level);
void func_088BF108(Unit29 *unit);
void func_088C5E44(Stats29 *stats);
void func_08A0B740(UnitRef29 *ref, Screen0B6A0 *self, Unit29 *unit);
void func_088BF238(Unit29 *unit, Record8CB164 *record, int arg);
void func_088C5E4C(Stats29 *stats, UnitRef29 *ref, int arg2, int arg3);
int func_089FA688(Screen0B6A0 *self, Unit29 *unit);
void func_088BF41C(Unit29 *unit);
void func_088BF17C(Unit29 *unit, int flags);
}

struct Fighters29 {
    static void SetLevel(Fighter29 *fighter, Record8CB164 *record) {
        if (fighter != 0) {
            func_088C07B4(func_088C09FC(fighter->kind), record->field_01);
        }
    }
};

extern "C" int func_089FA6E0(Screen0B6A0 *self, int id, unsigned char apply)
{
    Unit29 unit;
    Stats29 stats;
    UnitRef29 ref;
    Record8CB164 *record = func_088CB164(D_089D712C->field_F58, id);
    Fighter29 *fighter = func_088B70C8(id);
    int points;

    if (fighter != 0) {
        Fighters29::SetLevel(fighter, record);
    }
    WORK_INT(self->work, 0x32E4) = id;
    WORK_INT(self->work, 0x32E8) = record->field_01;
    WORK_INT(self->work, 0x32EC) = func_088CBAD0(record);
    WORK_INT(self->work, 0x32F0) = 1;
    func_088BF108(&unit);
    func_088C5E44(&stats);
    func_08A0B740(&ref, self, &unit);
    func_088BF238(&unit, record, 0);
    func_088C5E4C(&stats, &ref, 0, 0);
    points = stats.points;
    if (points > 999999) {
        points = 999999;
    }
    if (apply) {
        WORK_INT(self->work, 0x32F4) = func_089FA688(self, &unit);
    }
    func_088BF41C(&unit);
    func_088BF17C(&unit, 2);
    return points;
}
