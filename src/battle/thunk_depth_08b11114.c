// Calls func_08B0D938 on the 16-byte record (from +0x6C) of the table at +0x14 of the object at +0x34, indexed by the word at +4 of b; returns 0.
extern void func_08B0D938(void *);
typedef struct { char pad[0x14]; char *tbl; } Inner08B11114;
typedef struct { char pad[0x34]; Inner08B11114 *in; } Obj08B11114;
int func_08B11114(Obj08B11114 *self, int *b)
{
    int *pi = b + 1;
    func_08B0D938(self->in->tbl + *pi * 16 + 0x6C);
    return 0;
}
