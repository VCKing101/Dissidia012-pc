/* Initialises the sound manager: allocates 0x19000 bytes (align 0x80) into field 4, registers them as "SOUND MANAGER" (align 0x40) into field 0, func_088A1A80(self), then the "SOUND_MANAGER::ExecTask" task (prio 100000) and the audio callbacks and setup calls. */
extern int D_089D69A8;
extern char D_089CAC10[];
extern char D_089CAC20[];
extern void *func_08882118(int size, int align);
extern void *func_0885C40C(void *mem, int size, char *name, int align);
extern void func_088A1A80(void *self);
extern void *func_08882274(int v);
extern void func_088823C4(void *w, int a, int prio, void (*fn)(void), char *name);
extern void func_088A1AE4(void);
extern void func_088A1B04(void);
extern void func_088A1B3C(void);
extern void func_08990DB4(void (*a)(void), void (*b)(void));
extern void func_0898FFB0(void);
extern void func_089913EC(int a, int b);
void func_088A1274(void **self)
{
    void *p;
    p = func_08882118(0x19000, 0x80);
    self[1] = p;
    p = func_0885C40C(p, 0x19000, D_089CAC10, 0x40);
    self[0] = p;
    func_088A1A80(self);
    p = func_08882274(D_089D69A8);
    func_088823C4(p, -1, 100000, func_088A1AE4, D_089CAC20);
    func_08990DB4(func_088A1B04, func_088A1B3C);
    func_0898FFB0();
    func_089913EC(0x200, 4);
}
