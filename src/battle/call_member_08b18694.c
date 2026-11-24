// If self and the pointer at +4 are non-null, calls func_08B186E4 on that pointer.
extern int func_08B186E4(void *);
typedef struct { int pad; void *p; } Obj08B18694;
void func_08B18694(Obj08B18694 *self)
{
    if (self == 0) {
        return;
    }
    if (self->p == 0) {
        return;
    }
    if (func_08B186E4(self->p) == 0) {
        return;
    }
    return;
}
