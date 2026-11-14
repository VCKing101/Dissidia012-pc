typedef struct Counters { int total; } Counters;
extern void func_0892C448(Counters *c, int a1, int a2, int a3, int t0, int t1, int t2, int t3, float scale, int s0, int s1, int s2, int s3);
void func_0892B238(Counters *c, int unused, int a, int b, int d, int e)
{
    func_0892C448(c, 0, a, b, 0, d, e, 0, -1.0f, -1, -1, -1, -1);
    c->total = 1;
}
