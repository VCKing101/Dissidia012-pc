/* Initialises the object: stores the argument at +4, zeroes +0 and +8, and stores func_0881CBB8(0, arg) at +0xC. */
struct Obj { int a; int arg; float t; int h; };
extern void func_0881BAA8(void);
extern int func_0881CBB8(int, int);
void func_08B4FA6C(struct Obj *self, int arg)
{
    self->arg = arg;
    self->t = 0.0f;
    self->a = 0;
    func_0881BAA8();
    self->h = func_0881CBB8(0, arg);
}
