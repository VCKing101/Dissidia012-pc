// Starts the owner's trail: copies the unit's position through temporaries into the owner and runs its three setup steps; -1 when inactive.
typedef struct V3 { float x, y, z; } V3;
typedef struct V4 { float x, y, z, w; } V4;
struct Self { unsigned char active; char pad[3]; char pos[0x34]; int unit; };
extern void func_08B33374(int unit);
extern void func_08B3339C(int unit);
extern void func_08837158(V4 *out, int unit);
extern void func_089A34C0(void *dst, V3 *src);
extern void func_08AC0894(struct Self *s);
extern void func_08AC0998(struct Self *s);
extern void func_08AC0A00(struct Self *s);

int func_08AC04F8(struct Self *self)
{
    struct { V3 c; int p0; V4 a; V3 b; int p1; V4 v; int p2[3]; } L;
    if (self->active == 0) {
        return -1;
    }
    func_08B33374(self->unit);
    func_08B3339C(self->unit);
    func_08837158(&L.v, self->unit);
    L.a = L.v;
    L.b = *(V3 *)&L.a;
    L.c = L.b;
    func_089A34C0(self->pos, &L.c);
    func_08AC0894(self);
    func_08AC0998(self);
    func_08AC0A00(self);
    return 0;
}
