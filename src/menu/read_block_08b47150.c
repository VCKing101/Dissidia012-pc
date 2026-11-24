/* Runs func_08935970 on the block with a handler, copies three halfwords (+8, +0xA, +0xC) into words of the owner's object at +0x18 and returns the data start (+0x10). */
typedef struct Block { char pad[8]; short a; short b; short c; short pad2; char data[1]; } Block;
typedef struct Dest { int pad0; int w4; int w8; int wc; int w10; int w14; int w18; } Dest;
typedef struct Owner { char pad[0x18]; Dest *dest; } Owner;
extern void func_08935970(Block *block, void (*fn)(void));
extern void D_08B48574(void);
char *func_08B47150(Owner *self, Block *block)
{
    Owner *o = self;
    Block *b = block;
    func_08935970(b, D_08B48574);
    o->dest->w8 = b->a;
    o->dest->wc = b->b;
    o->dest->w10 = b->c;
    return b->data;
}
