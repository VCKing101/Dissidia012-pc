/* Creates the 0x6F4-byte menu object (func_089110C4 + func_08B49890) into field 0x570, calls func_08B49914(obj, field 4, 1, 200), sets byte 0x7E9 and registers the "CockpitMenuUpdate" task (func_089177B4) on field 4. */
typedef struct Obj08916298 { unsigned char pad0[4]; void *owner; unsigned char pad1[0x570 - 8]; void *menu; unsigned char pad2[0x7E9 - 0x574]; unsigned char ready; } Obj08916298;
extern char D_089CE3D4[];
extern void *func_089110C4(int size);
extern void func_08B49890(void *p);
extern void func_08B49914(void *p, void *owner, int a, int b);
extern void func_089177B4(void);
extern void func_088823C4(void *owner, int a, int b, void (*fn)(void), char *name);
void func_08916298(Obj08916298 *self)
{
    void *m;
    void *r;
    self->menu = func_089110C4(0x6F4);
    m = self->menu;
    r = 0;
    if (m != 0) {
        func_08B49890(m);
        r = m;
    }
    self->menu = r;
    func_08B49914(r, self->owner, 1, 200);
    self->ready = 1;
    func_088823C4(self->owner, 1, 200, func_089177B4, D_089CE3D4);
}
