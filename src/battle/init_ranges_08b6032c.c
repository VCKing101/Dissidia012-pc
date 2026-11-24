/* Initialises the object with a value at +4 and two floats from func_08864CB8/func_08864CC0 of func_08B5EE40's result. */
struct Obj { int a; int b; int c; unsigned char d; char pad[3]; float lo; float hi; float e; };
extern void *func_08B5EE40(void);
extern float func_08864CB8(void *);
extern float func_08864CC0(void *);
void func_08B6032C(struct Obj *self, int b)
{
    self->a = 0;
    self->b = b;
    self->e = 0.0f;
    self->d = 0;
    self->c = 0;
    self->lo = func_08864CB8(func_08B5EE40());
    self->hi = func_08864CC0(func_08B5EE40());
}
