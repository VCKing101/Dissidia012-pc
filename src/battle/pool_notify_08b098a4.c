/* Passes the result of func_088D4C5C to the pool at D_089D6C30 when the object holds a block. */
typedef struct PoolObj2 { int pad[4]; int block; } PoolObj2;
extern int D_089D6C30;
extern int func_088D4C5C(void);
extern void func_0889F340(int pool, int v);
void func_08B098A4(PoolObj2 *self)
{
    if (self->block != 0) {
        int pool = D_089D6C30;
        func_0889F340(pool, func_088D4C5C());
    }
}
