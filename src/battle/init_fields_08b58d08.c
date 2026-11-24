/* Initialises an object: copies the sub-struct argument via func_089A34C0 and stores two floats and a word. */
extern void func_089A34C0(void *, int);
struct Obj { float a; float b; int c; char sub[1]; };
void func_08B58D08(float a, float b, int unused, struct Obj *self, int e, int f)
{
    int *pc;
    func_089A34C0(self->sub, e);
    self->a = a;
    self->b = b;
    pc = &self->c;
    *pc = f;
}
