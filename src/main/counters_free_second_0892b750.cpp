extern "C" {
typedef struct Counters { int total; void *first[1]; void *second[7]; } Counters;
extern void func_088DA060(void *p);
void func_0892B750(Counters *c)
{
    for (int i = 0; i < 7; i++) {
        if (c->second[i] != 0) {
            func_088DA060(c->second[i]);
            c->second[i] = 0;
        }
    }
}
}
