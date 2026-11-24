// Calls func_08B18838(a, b); if either counter (+0x10 or +0x74 of the record at *b + 8) is positive and the result is not 1, returns 3 instead.
extern int func_08B18838(void *, void *);
typedef struct { char pad0[8]; int c10; char pad1[0x74 - 0xC]; int c74; } Rec08B18ac4;
int func_08B18AC4(void *a, Rec08B18ac4 **b)
{
    Rec08B18ac4 *r = *b;
    int v;
    r = (Rec08B18ac4 *)((char *)r + 8);
    v = func_08B18838(a, b);
    if (*(int *)((char *)r + 0x10) > 0 || *(int *)((char *)r + 0x74) > 0) {
        if (v != 1) {
            v = 3;
        }
    }
    return v;
}
