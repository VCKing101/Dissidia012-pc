/* Picks one of two parameter sets by func_08B0DF10 and starts the effect with func_088FD928, using the handle at +0x89C of the owner's block and the one at +0x128 of this one's. */
typedef struct Own08B0B87C { char pad[0x89C]; int h89C; } Own08B0B87C;
typedef struct Glob08B0B87C { char pad[0x140]; Own08B0B87C *own; } Glob08B0B87C;
typedef struct Blk08B0B87C { char pad[0x128]; int h128; } Blk08B0B87C;
typedef struct S08B0B87C { char pad[0x134]; int w134; Blk08B0B87C *blk; } S08B0B87C;
extern Glob08B0B87C *D_08B01CF8;
extern int D_089D7F90;
extern char D_08B1C662[];
extern char D_08B1C67A[];
extern int func_08B0DF10(S08B0B87C *self);
extern int func_08B10424(Own08B0B87C *o);
extern void func_088FD928(int a, int b, int c, int d, char *e, int f, int g, int h, int i, int j, int k, int l);
void func_08B0B87C(S08B0B87C *self)
{
    int c = 0xF;
    char *name = D_08B1C662;
    int n = 5;
    int id = 0x1C73;
    int w;
    int g;
    int o;
    if (func_08B0DF10(self) != 0) {
        c = 0x16;
        name = D_08B1C67A;
        n = 8;
        id = 0x1C7C;
    }
    w = self->w134;
    {
        int h = self->blk->h128;
        Own08B0B87C *own = D_08B01CF8->own;
        g = D_089D7F90;
        o = own->h89C;
        func_088FD928(g, w, c, 8, name, id, o, n, h, 0, 0, func_08B10424(own));
    }
}
