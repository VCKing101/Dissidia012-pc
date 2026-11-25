/* replace the block's effect handle and place it at the target's position with a given scale. */
extern void func_088A6C70(void *h);
extern void *func_08B421FC(void *a, int b);
extern void func_088A7284(void *h, float x, float y, float z);
extern void *D_08B42288;
typedef struct Pos { char pad[0x80]; float x, y, z; } Pos;
typedef struct Eff { char pad[0xE0]; float scale; } Eff;
typedef struct Blk { int a0; Pos *target; char pad[0x44 - 8]; Eff *h; } Blk;
typedef struct Obj { char pad[0xC]; Blk *blk; } Obj;
void func_08921BA4(Obj *self, float scale, int kind)
{
    if (self->blk->h != 0) {
        func_088A6C70(self->blk->h);
        self->blk->h = 0;
    }
    self->blk->h = func_08B421FC(D_08B42288, kind);
    {
        Pos *p = self->blk->target;
        func_088A7284(self->blk->h, p->x, p->y, p->z);
    }
    self->blk->h->scale = scale;
}
