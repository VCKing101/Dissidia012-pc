/* Starts the object: calls func_08B6096C with the result of func_088985F0(func_08899300(), 1), then clears the float at +0xC. */
struct Obj { char pad[0xC]; float t; };
extern void *func_08899300(void);
extern void *func_088985F0(void *, int);
extern void func_08B6096C(struct Obj *, void *, int);
void func_08B60660(struct Obj *self)
{
    void *a = func_08899300();
    void *b = func_088985F0(a, 1);
    func_08B6096C(self, b, 1);
    self->t = 0.0f;
}
