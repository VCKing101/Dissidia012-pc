/* Starts the unit's motion blend when its model accepts it. */
typedef struct UnitModel {
    char pad_00[0x130];
    char motion[4];             /* 0x130 */
} UnitModel;

typedef struct UnitBody {
    char pad_00[0xBC];
    UnitModel *model;           /* 0xBC */
} UnitBody;

typedef struct Unit {
    char pad_00[0x20];
    UnitBody *body;             /* 0x20 */
} Unit;

extern float D_089C7C2C;
extern int func_08B348A4(UnitModel *model, int kind);
extern void func_0882FA10(void *motion, int kind, int flags, float rate, float start);

void func_08922D3C(Unit *unit)
{
    UnitModel *model = unit->body->model;
    if (model == 0) {
        return;
    }
    if (func_08B348A4(model, 1) == 0) {
        return;
    }
    func_0882FA10(unit->body->model->motion, 1, 0, D_089C7C2C, 0.0f);
}
