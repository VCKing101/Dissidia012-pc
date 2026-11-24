// Runs func_08B483F0, stores a1/a2 at +0x50/+0x54, builds two objects with func_0887B3EC (stored at +0x48 and +0x4C) and registers them with func_088D428C.
extern void func_08B483F0(void *);
extern int func_0887B3EC(int, int, int, int, int, int);
extern void func_088D428C(void *, void *, void (*)(void), int);
extern void func_08B484D8(void);
typedef struct { char pad0[0x38]; char a[0x10]; int h48; int h4c; int w50; int w54; } Obj08B48354;
void func_08B48354(Obj08B48354 *self0, int a10, int a20)
{
    int a2 = a20;
    Obj08B48354 *self = self0;
    int a1 = a10;
    func_08B483F0(self);
    self->w50 = a1;
    self->w54 = a2;
    self->h48 = func_0887B3EC(a1, a2, 0, 1, 0, 0);
    self->h4c = func_0887B3EC(a1, a2, 0, 0, 0, 0);
    func_088D428C(self->a, &self->h48, func_08B484D8, 1);
}
