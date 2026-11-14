extern "C" {
typedef struct Counters { int total; void *first[1]; void *second[7]; } Counters;
extern void func_088DAB44(void *p);
void func_0892B6FC(Counters *c)
{
    for (int i = 0; i < 1; i++) {
        if (c->first[i] != 0) {
            func_088DAB44(c->first[i]);
            c->first[i] = 0;
        }
    }
}
}
