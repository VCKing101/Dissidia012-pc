/* Binds a task record to an update callback created through func_088823C4; -1 bad source, -2 creation failed. */
typedef struct BTask { unsigned char on; char pad[3]; int link; } BTask;
typedef struct BSrc { char pad[8]; int link; } BSrc;
typedef struct BObj { char pad[4]; BTask *owner; } BObj;
extern char D_08B18824[];
extern void func_08B18590(void);
extern void func_08B18694(void);
extern BObj *func_088823C4(int a, int b, int c, void (*fn)(void), char *name);
int func_08B185D0(BTask *self, BSrc *src, int a, int b, int c)
{
    BObj *o;
    BTask *me = self;
    BSrc *sr = src;
    func_08B18590();
    if (sr == 0 || sr->link == 0 || a == 0) {
        return -1;
    }
    o = func_088823C4(a, b, c, func_08B18694, D_08B18824);
    if (o == 0) {
        return -2;
    }
    o->owner = me;
    me->link = sr->link;
    me->on = 1;
    return 0;
}
