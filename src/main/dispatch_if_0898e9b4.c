// If func_0898EBFC yields a handle, calls func_0898EA14 with the object's two floats and halfword.
struct O { char d[8]; float x; float y; short h; };
extern int func_0898EBFC(struct O *o);
extern void func_0898EA14(struct O *o, int b, int h, int v, float x, float y);
void func_0898E9B4(struct O *o, int b)
{
    int v = func_0898EBFC(o);
    if (v == 0) {
        return;
    } else {
        float x = o->x;
        float y = o->y;
        func_0898EA14(o, b, o->h, v, x, y);
    }
}
