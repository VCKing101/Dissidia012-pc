/* Calls virtual 8 on every unit of the scene while the renderer is up. */
struct Unit08841218 {
    char pad_000[0x4A4];
    Unit08841218 *next;
    char pad_4A8[0x9C];
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void refresh();
};

struct Scene08841218 {
    char pad_00[0x3C];
    Unit08841218 *units;

    static Unit08841218 *after(Scene08841218 *scene, Unit08841218 *u)
    {
        return u == 0 ? scene->units : u->next;
    }
};

extern "C" {

extern int D_09AFEE54;
extern Scene08841218 *D_089D4958;

void func_08876330(void);
void func_08876370(void);

void func_08841218(void)
{
    if (D_09AFEE54 == 0) {
        return;
    }
    func_08876330();
    Scene08841218 *scene = D_089D4958;
    Unit08841218 *u = 0;
    while ((u = Scene08841218::after(scene, u)) != 0) {
        u->refresh();
    }
    func_08876370();
}

}
