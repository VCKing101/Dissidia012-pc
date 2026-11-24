// Calls func_088D9CFC with the pointer at +0x3C when it is non-null.
extern void func_088D9CFC(void *);
typedef struct { char pad[0x3C]; void *p; } Obj08B47E18;
void func_08B47E18(Obj08B47E18 *self)
{
    if (self->p == 0) {
        return;
    }
    func_088D9CFC(self->p);
}
