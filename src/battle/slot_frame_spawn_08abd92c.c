// Spawns the effect for motion slot kind 1 between two units: builds a frame from their offset and hands it on; -1 when inactive, -2 when a unit is missing.
typedef struct V4 { float x, y, z, w; } V4 __attribute__((aligned(16)));
typedef struct Owner { unsigned char active; void *kinds; void *sink; } Owner;
typedef struct Unit { char pad[0x100]; char mat[1]; } Unit;
extern void func_089A38FC(V4 *dst, void *a, void *b);
extern void func_089A3BC8(V4 *dst, V4 *src);
extern void func_089A383C(V4 *dst, float x, float y, float z, float w);
extern void func_089A3B30(V4 *dst, V4 *a, V4 *b);
extern float func_089A3BF8(V4 *v);
extern void *func_08ABC300(void *kinds, int kind);
extern int func_08ABDD0C(Owner *self, Unit *a, Unit *b);
extern void func_08ABFA4C(void *sink, int a, V4 *p, V4 *q, V4 *r, void *kind, int sel);

int func_08ABD92C(Owner *self, Unit *from, Unit *to, int arg)
{
    V4 dir __attribute__((aligned(16)));
    V4 up __attribute__((aligned(16)));
    V4 side __attribute__((aligned(16)));
    if (self->active == 0) {
        return -1;
    }
    if (from == 0 || to == 0) {
        return -2;
    }
    func_089A38FC(&dir, to->mat, from->mat);
    func_089A3BC8(&dir, &dir);
    func_089A383C(&up, 0.0f, 1.0f, 0.0f, 1.0f);
    func_089A3B30(&side, &dir, &up);
    if (func_089A3BF8(&side) == 0.0f) {
        func_089A383C(&up, 1.0f, 0.0f, 0.0f, 1.0f);
        func_089A3B30(&side, &dir, &up);
    }
    func_089A3BC8(&side, &side);
    func_089A3B30(&up, &dir, &side);
    {
        void *kind = func_08ABC300(self->kinds, 1);
        int sel = func_08ABDD0C(self, from, to);
        func_08ABFA4C(self->sink, arg, &dir, &up, &side, kind, sel);
    }
    return 0;
}
