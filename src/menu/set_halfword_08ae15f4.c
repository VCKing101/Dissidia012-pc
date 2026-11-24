/* Stores a halfword at index func_08AE150C() of the table at self->p850+0x624, then calls func_08AE20B0(self). */
typedef struct { char p0[0x624]; short t[1]; } Tab;
typedef struct { char p0[0x850]; Tab *p850; } Obj;
extern int func_08AE150C(void);
extern void func_08AE20B0(void *self);
void func_08AE15F4(Obj *self, int v) {
    Obj *me = self;
    int val = v;
    int i = func_08AE150C();
    me->p850->t[i] = val;
    func_08AE20B0(me);
}
