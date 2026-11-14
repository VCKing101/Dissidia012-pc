typedef struct Cnt {
    char pad0[0xE];
    unsigned short count;
} Cnt;

unsigned short func_089BA814(Cnt *p)
{
    unsigned short v = 0;
    if (p != 0) {
        v = p->count + 1;
        p->count = v;
    }
    return v;
}
