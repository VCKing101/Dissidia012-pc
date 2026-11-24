/* Plays the animation for the current step on the owner object; at the step's frame also mirrors the position. */
struct Obj { float f0; char d4[8]; short s0C; };
struct Self { char d0[8]; struct Obj *obj; char dC[8]; int step; };
extern void func_088D9F5C(struct Obj *o, int anim, int flag);
extern void func_088D9AEC(float v, struct Obj *o);
extern int D_08B05BA0[];
extern int D_08B05BB0[];
extern float D_08B05B9C;

int func_08B05AD8(struct Self *s)
{
    struct Obj *o = s->obj;
    if (o == 0) {
        return 0;
    }
    if (o->s0C == D_08B05BA0[s->step]) {
        float r = D_08B05B9C - o->f0;
        func_088D9F5C(o, D_08B05BB0[s->step], 1);
        func_088D9AEC(r, s->obj);
    } else {
        func_088D9F5C(o, D_08B05BB0[s->step], 1);
    }
    return 0;
}
