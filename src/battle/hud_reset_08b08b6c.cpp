// After func_088F47E0(0x16) reports none, with arg 1 and a target at +0xD4, kicks the effect at +0x388C with its id and 180.0; then runs func_08B098E4(+0xA4).
extern "C" {
extern int func_088F47E0(int);
extern void func_08B1209C(void *, int);
extern void func_08B122CC(float, void *);
extern void func_08B098E4(void *);
}
typedef struct Tgt57 {
    char pad0[0x10];
    short id;
} Tgt57;
struct Has57 { static bool has(int v) { return v != 0; } };
typedef struct Hud57 {
    char pad0[0xD4];
    Tgt57 *tgt;
} Hud57;
extern "C" void func_08B08B6C(Hud57 *self, unsigned char mode)
{
    if (!Has57::has(func_088F47E0(0x16)) && mode == 1 && self->tgt != 0) {
        char *fx = (char *)self + 0x388C;
        func_08B1209C(fx, self->tgt->id);
        func_08B122CC(180.0f, fx);
    }
    func_08B098E4((char *)self + 0xA4);
}
