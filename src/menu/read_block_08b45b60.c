/* Runs func_08935970 on the block with a handler, stores the halfword at +8 into the owner's word at +0xD00 and returns the data start (+0x10). */
typedef struct Block { char pad[8]; short v; char pad2[6]; char data[1]; } Block;
typedef struct Owner { int *w; } Owner;
extern void func_08935970(Block *block, void (*fn)(void));
extern void D_08B45BB0(void);
char *func_08B45B60(Owner *self, Block *block)
{
    Owner *o = self;
    Block *b = block;
    func_08935970(b, D_08B45BB0);
    o->w[0xD00 / 4] = b->v;
    return b->data;
}
