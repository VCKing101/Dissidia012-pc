/* When exactly one of two unit flags is set (7 without 5, or 5 without 7) stores a fresh roll in the object; returns 0. */
typedef struct Obj38 { char pad[0x20]; float roll; char pad2[0x13C]; void *h160; } Obj38;
extern void *func_08AB3274(void *h);
extern int func_08A82684(void *u, int id);
extern int func_08A82638(void *u, int id);
extern float func_08AA578C(int p);
extern int D_08AA5828;
int func_08A9C780(Obj38 *o)
{
    void *u = func_08AB3274(o->h160);
    if ((func_08A82684(u, 7) == 1 && func_08A82638(u, 5) == 0) || (func_08A82684(u, 5) == 1 && func_08A82638(u, 7) == 0)) {
        o->roll = func_08AA578C(D_08AA5828);
    }
    return 0;
}
