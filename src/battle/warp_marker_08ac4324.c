/* Returns the lookup of the name after "warp" when the entry's tag starts with it and the timer is below 0.01; -1 otherwise. */
typedef struct Reg38 { char pad[0x70]; char *entries; } Reg38;
typedef struct Obj38 { char pad[0x3C0]; float timer; short index; } Obj38;
extern Reg38 *func_08899300();
extern int func_08934924(char *s);
int func_08AC4324(int unused, Obj38 *o)
{
    char *p;
    p = func_08899300()->entries + o->index * 0x4C;
    if (o->timer < 0.01f && p[0] == 'w' && p[1] == 'a' && p[2] == 'r' && p[3] == 'p') {
        return func_08934924(p + 5);
    } else {
        return -1;
    }
}
