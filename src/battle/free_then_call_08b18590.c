// Frees the pointer at +8 with func_088DA060 when set (clearing it), then calls func_08B18580(self).
extern void func_088DA060(void *);
extern void func_08B18580(void *);
typedef struct { char pad[8]; void *p; } Obj08B18590;
void func_08B18590(Obj08B18590 *self)
{
    if (self->p != 0) {
        func_088DA060(self->p);
        self->p = 0;
    }
    func_08B18580(self);
}
