// Spawns the effect for motion slot kind 0xC of the owner: builds the orientation from the unit's matrix and hands it on; -1 when inactive, -2 when no unit.
typedef struct V4 { float x, y, z, w; } V4 __attribute__((aligned(16)));
typedef struct Owner { unsigned char active; void *kinds; void *sink; } Owner;
typedef struct Unit { char pad[0x90]; char mat[1]; } Unit;
extern int func_08ABDDD0(Owner *self, Unit *unit);
extern void func_089A3868(V4 *dst, void *src);
extern void func_089A383C(V4 *dst, float x, float y, float z, float w);
extern void func_089A3B30(V4 *dst, V4 *a, V4 *b);
extern void *func_08ABC300(void *kinds, int kind);
extern int func_08ABDC60(Owner *self, Unit *unit, int arg);
extern void func_08ABFA4C(void *sink, int a, V4 *p, V4 *q, V4 *r, void *kind, int sel);

int func_08ABD810(Owner *self, Unit *unit, int a, int b)
{
    V4 m __attribute__((aligned(16)));
    V4 up __attribute__((aligned(16)));
    V4 out __attribute__((aligned(16)));
    if (self->active == 0) {
        return -1;
    }
    if (unit == 0) {
        return -2;
    }
    if (func_08ABDDD0(self, unit) == 0) {
        return 0;
    }
    func_089A3868(&m, unit->mat);
    func_089A383C(&up, 0.0f, 1.0f, 0.0f, 1.0f);
    func_089A3B30(&out, &m, &up);
    {
        void *kind = func_08ABC300(self->kinds, 0xC);
        int sel = func_08ABDC60(self, unit, b);
        func_08ABFA4C(self->sink, a, &m, &up, &out, kind, sel);
    }
    return 0;
}
