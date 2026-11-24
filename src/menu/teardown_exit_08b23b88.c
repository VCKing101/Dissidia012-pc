/* Tears down a menu state: calls func_088D0370(D_089D7830, 0x10, 1), frees self->0x138 via func_0885C538 when self->0x134 is set, calls func_08AF7CD0(D_08B01CF8), then the callback fetched from the freed object and clears 0x09B6DA5C. */
typedef void (*Fn)(int);
struct Obj { Fn fn; };
struct Self { char pad[0x134]; void *owner; struct Obj *obj; };
extern void *D_089D7830;
extern void *D_08B01CF8;
extern int D_09B6DA58[3];
extern void func_088D0370(void *a, int b, int c);
extern void func_0885C538(void *owner, struct Obj *obj, int z);
extern void func_08AF7CD0(void *p);
void func_08B23B88(struct Self *self)
{
    Fn fn;
    int kind = 0x10;
    func_088D0370(D_089D7830, kind, 1);
    fn = self->obj->fn;
    if (self->owner != 0) {
        func_0885C538(self->owner, self->obj, 0);
        self->obj = 0;
    }
    func_08AF7CD0(D_08B01CF8);
    fn(0);
    D_09B6DA58[1] = 0;
}
