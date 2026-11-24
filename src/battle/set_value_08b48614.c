// If a >= 0, stores it at +0x44 of the object D_08B491E8 points to and passes it to func_08912FA4.
extern int func_08912FA4(int);
typedef struct { char pad[0x44]; int v; } Obj08B48614;
extern Obj08B48614 *volatile D_08B491E8;
void func_08B48614(int a)
{
    if (a < 0) {
        return;
    }
    D_08B491E8->v = a;
    if (func_08912FA4(D_08B491E8->v) == 0) {
        return;
    }
    return;
}
