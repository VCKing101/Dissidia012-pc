// Calls func_088D9CD8 on the pointer at +8 when set, then func_088D9F5C(c, 5, 1) on the pointer at +0xC when set.
extern void func_088D9CD8(void *);
extern void func_088D9F5C(void *, int, int);
typedef struct { char pad[8]; void *p8; void *pc; } Obj08B4753C;
void func_08B4753C(Obj08B4753C *self)
{
    if (self->p8 != 0) {
        func_088D9CD8(self->p8);
    }
    {
        void *c = self->pc;
        if (c != 0) {
            func_088D9F5C(c, 5, 1);
        }
    }
}
