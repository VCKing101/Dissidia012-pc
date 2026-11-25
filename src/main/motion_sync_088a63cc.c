typedef struct O { char pad[8]; void *h; char pad2[4]; char a_sub[0xC0]; char b_sub[1]; } O;
extern int func_0885EA88(void *p);
extern int func_0885EA90(void *p);
extern int func_088735C4(void *p);
extern float func_0887357C(void *p);
extern float func_08873584(void *p);
extern void func_08941174(void *h, int a, int b);
extern void func_08941194(void *h, int a);
extern void func_089411B8(void *h, float x, float y);
void func_088A63CC(O *o) {
    int v1 = func_0885EA88(o->a_sub);
    char *s = o->b_sub;
    int v2 = func_088735C4(s);
    func_08941174(o->h, v1, v2);
    void *h = o->h;
    int v3 = func_0885EA90(o->a_sub);
    func_08941194(h, v3);
    void *h2 = o->h;
    float x = func_0887357C(s);
    float y = func_08873584(s);
    func_089411B8(h2, x, y);
}
