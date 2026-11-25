struct V4_7BCC8 { float v[4]; } __attribute__((aligned(16)));
struct P7BCC8 { char pad[0x50]; float a; float b; };
extern "C" void func_089A38E0(void *, void *, void *);
extern "C" void func_089A3A44(void *, void *, float);
extern "C" void func_089A38A8(void *, void *, void *);
extern "C" void func_0897BCC8(char *o1, char *o2, char *s1, char *s2, P7BCC8 *p)
{
    V4_7BCC8 t1;
    V4_7BCC8 t2;
    float f = p->a + p->b * 2.0f;
    if (f == 0.0f) {
        f = 1.0f;
    } else {
        f = p->b / f;
    }
    func_089A38E0(&t1, s1 + 0x10, s1);
    func_089A3A44(&t1, &t1, f);
    func_089A38A8(o1, s1, &t1);
    func_089A38E0(o1 + 0x10, s1 + 0x10, &t1);
    func_089A38E0(&t2, s2 + 0x10, s2);
    func_089A3A44(&t2, &t2, f);
    func_089A38A8(o2, s2, &t2);
    func_089A38E0(o2 + 0x10, s2 + 0x10, &t2);
}
