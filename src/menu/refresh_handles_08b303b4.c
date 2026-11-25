/* Runs the base routine, then derives three handles from the row picked by the indices at +0x938/+0x964 of the block and refreshes the panel. */
extern void func_08B3045C(void *self);
extern int func_0887E534(int h);
extern void func_088D428C(int a, int b, const char *c, int d);
extern const char D_08B31826[];
typedef struct Blk08B303B4 {
    char pad0[0x5F8];
    int w5F8;
    int w5FC;
    int w600;
} Blk08B303B4;
typedef struct S08B303B4 { char pad[0x9D8]; Blk08B303B4 *blk; } S08B303B4;
void func_08B303B4(S08B303B4 *self)
{
    int idx;
    int off;
    func_08B3045C(self);
    idx = *(int *)((char *)self->blk + 0x938) * 2 + *(int *)((char *)self->blk + 0x964);
    off = idx * 4;
    self->blk->w5F8 = func_0887E534(*(int *)((char *)self->blk + off + 0x604));
    self->blk->w5FC = func_0887E534(*(int *)((char *)self->blk + off + 0x604));
    self->blk->w600 = func_0887E534(*(int *)((char *)self->blk + off * 4 + 0x504));
    func_088D428C((int)self->blk + 0x10, (int)self->blk + 0x490, D_08B31826, 1);
}
