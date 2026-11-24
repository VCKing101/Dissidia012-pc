/* Hands the object's block and owner back to the pool at D_089D6C30, if it holds a block. */
typedef struct PoolObj { int owner; int pad4; int pad8; int padc; int block; int pad14; int size; } PoolObj;
extern int D_089D6C30;
extern void func_0889F290(int pool, int block, int size);
extern void func_0889F2D4(int pool, int owner);
void func_08B09844(PoolObj *self)
{
    if (self->block != 0) {
        int pool = D_089D6C30;
        func_0889F290(pool, (self->block + 0x3F) & -0x40, self->size);
        func_0889F2D4(pool, self->owner);
    }
}
