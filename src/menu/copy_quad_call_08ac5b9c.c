/* Copies the 16-byte record D_09B698C8 into self->p138+0x9F4, then runs four update calls with that record. */
typedef struct { int w[4]; } Quad;
typedef struct { char p0[0x9F4]; Quad q; } Blk;
typedef struct { char p0[0x138]; Blk *p138; } Obj;
extern Quad D_09B698C8;
extern void func_08AC4858(Obj *o, Quad *q);
extern void func_08AC58D0(Obj *o);
extern void func_08AC4FE8(Obj *o, Quad *q);
extern void func_08AC523C(Obj *o, Quad *q);
void func_08AC5B9C(Obj *self) {
    Obj *me = self;
    me->p138->q = D_09B698C8;
    func_08AC4858(me, &me->p138->q);
    func_08AC58D0(me);
    func_08AC4FE8(me, &me->p138->q);
    func_08AC523C(me, &me->p138->q);
}
