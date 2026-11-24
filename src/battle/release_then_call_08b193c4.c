// Releases the pointer at +8 with func_08890970 when set, then calls func_08B193B0(self).
extern void func_08890970(void *);
extern void func_08B193B0(void *);
typedef struct { char pad[8]; void *p; } Obj08B193C4;
void func_08B193C4(Obj08B193C4 *self)
{
    if (self->p != 0) {
        func_08890970(self->p);
    }
    func_08B193B0(self);
}
