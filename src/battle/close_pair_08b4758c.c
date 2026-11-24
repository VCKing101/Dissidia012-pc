// Calls func_088D9CFC on the pointer at +8 when set, then func_088D9F5C(c, 4, 1) on the pointer at +0xC when set.
extern void func_088D9CFC(void *);
extern void func_088D9F5C(void *, int, int);
typedef struct { char pad[8]; void *p8; void *pc; } Obj08B4758C;
void func_08B4758C(Obj08B4758C *self)
{
    if (self->p8 != 0) {
        func_088D9CFC(self->p8);
    }
    {
        void *c = self->pc;
        if (c != 0) {
            func_088D9F5C(c, 4, 1);
        }
    }
}
