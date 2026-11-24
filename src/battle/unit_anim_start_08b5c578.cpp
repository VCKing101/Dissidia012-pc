// Starts the unit's animation state: applies the colour vector, clears its timers, plays motion 0x65 and notifies a pending child.
struct V4 { float x, y, z, w; };
struct Mode { int kind; };
struct Unit {
    char pad0[0xAC]; float f_ac;
    char pad1[0x130 - 0xB0]; char motion[1];
    char pad2[0x3A4 - 0x131]; Mode *mode;
    char pad3[0xD40 - 0x3A8]; char child[4]; int childPtr;
};

struct Present {
    static bool is(int v) { return v != 0; }
};

extern "C" {

extern void func_088359A8(Unit *u, V4 *col, unsigned char a);
extern void func_08834C10(Unit *u, int a, int b);
extern void func_08A5BE70(float v, Unit *u);
extern void func_08837A5C(Unit *u);
extern void func_088348C8(Unit *u, void *p);
extern void func_08834934(Unit *u, int a, int b);
extern void func_0882FA10(void *motion, int id, float x, float y, int arg);
extern void func_088118F8(void *child);
extern char D_09AF84C0[];

void func_08B5C578(int unused, Unit *u, V4 *col)
{
    V4 c;
    float zero;
    c = *col;
    func_088359A8(u, &c, 1);
    func_08834C10(u, 0x12, 0);
    zero = 0.0f;
    func_08A5BE70(zero, u);
    func_08837A5C(u);
    u->f_ac = zero;
    if (u->mode->kind == 1) {
        func_088348C8(u, D_09AF84C0);
    }
    func_08834934(u, 0x65, 0x66);
    func_0882FA10(u->motion, 0x65, zero, zero, 0);
    if (Present::is(u->childPtr)) {
        func_088118F8(u->child);
    }
}

}
