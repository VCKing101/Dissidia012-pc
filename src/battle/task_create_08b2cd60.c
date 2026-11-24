/* Resets the task, registers an update callback through func_088823C4 pointing back at the task, and initialises its fields. */
typedef struct Task { int a; int b; int c; char pad_0c[8]; unsigned char f14; unsigned char f15; } Task;
typedef struct TaskObj { char pad[4]; Task *owner; } TaskObj;
extern int D_089D6A10;
extern char D_08B2D290[];
extern void func_08B2CE04(void);
extern void func_08B2CF5C(void);
extern TaskObj *func_088823C4(int a, int b, int c, void (*fn)(void), char *name);
extern int func_08889B08(int v);
int func_08B2CD60(Task *self, int a, int b, int c, int d)
{
    TaskObj *o;
    Task *me = self;
    int aa = a;
    func_08B2CE04();
    o = func_088823C4(aa, b, c, func_08B2CF5C, D_08B2D290);
    o->owner = me;
    me->b = func_08889B08(D_089D6A10);
    me->a = d;
    me->c = 1;
    me->f15 = 1;
    me->f14 = 1;
    return 0;
}
