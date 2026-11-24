// If the pointer at *self is set, releases it with func_08890970 and clears it.
extern void func_08890970(void *);
typedef struct { void *p; } Obj08B0DAD8;
void func_08B0DAD8(Obj08B0DAD8 *self)
{
    if (self->p != 0) {
        func_08890970(self->p);
        self->p = 0;
    }
}
