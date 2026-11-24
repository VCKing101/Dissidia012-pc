/* Sets up the page from the global object: copies its id and count, resolves the record and refreshes the page parts. */
extern void func_08AE4314(void);
extern void func_08AE0F70(void *self, int a, int b);
extern void func_08AE3B14(void *self, int a, int b, int id);
extern void *func_088CB164(void *g, int id);
extern void func_08AE1968(void *self, int a, int b);
extern void func_08AE2614(void *self, int a);
extern void func_08AE21F4(void *self, int a);
extern char *D_089D712C;

typedef struct Ob021C { int first; char pad[0x2BF8]; int cnt; } Ob021C;
typedef struct O021C { char pad[0x140]; Ob021C *objs; } O021C;
typedef struct Sub021C { char pad[0x763]; char kind; } Sub021C;
typedef struct Rec021C { char pad[0x10]; Sub021C sub; } Rec021C;
extern O021C *D_08B01CF8;
typedef struct B021C { char pad[0x5A4]; unsigned flags; char pad5A8[4]; int id; int count; int kind; } B021C;
typedef struct S021C { char pad[0x850]; B021C *blk; } S021C;

void func_08AE021C(S021C *self)
{
    Rec021C *rec;
    func_08AE4314();
    func_08AE0F70(self, 0, -1);
    self->blk->id = D_08B01CF8->objs->first;
    func_08AE3B14(self, 1, 0, self->blk->id);
    self->blk->count = D_08B01CF8->objs->cnt;
    if ((self->blk->flags & 8) != 0 && self->blk->count <= 0) {
        self->blk->count = 1;
    }
    rec = (Rec021C *)func_088CB164(D_089D712C + 0xF58, self->blk->id);
    {
        Sub021C *sp = &rec->sub;
        self->blk->kind = sp->kind;
    }
    func_08AE1968(self, 1, self->blk->kind);
    func_08AE2614(self, 0);
    func_08AE21F4(self, 0);
}
