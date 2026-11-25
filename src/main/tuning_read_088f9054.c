/* read the six tuning values into the object's block. */
extern int func_088F0AFC(int a, int b);
extern float func_088F0C18(int a);
typedef struct Obj { char pad[0x85C]; float f85C, f860, f864, f868; short h86C, h86E; } Obj;
void func_088F9054(Obj *s)
{
    s->f85C = (float)func_088F0AFC(3, 1) / 100.0f;
    s->f860 = func_088F0C18(5);
    s->f864 = func_088F0C18(4);
    s->f868 = (float)func_088F0AFC(2, 1) / 100.0f;
    s->h86C = func_088F0AFC(0, 1);
    s->h86E = func_088F0AFC(1, 1);
}
