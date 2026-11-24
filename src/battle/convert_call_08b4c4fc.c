// Unless a -1 id is passed: looks up the record with func_088C0FE8, combines four short-valued entries with func_08B4B040 and a derived value with func_0887AD78, and passes both to func_08B4C264.
extern void *func_088C0FE8(int);
extern int func_08B4B040(int, int, int, int, int, int);
extern int func_0887AD78(int, int, int, int, int, int);
extern void func_08B4C264(int, int, int, int);
typedef struct { int pad0; short *p10; int pad1; short *p18; int pad2; short *p20; int pad3; short *p28; } Ptrs08B4C4FC;
typedef struct { char pad0[8]; short s8; char pad1[0xC - 0xA]; Ptrs08B4C4FC q; } Rec08B4C4FC;
void func_08B4C4FC(int a0, int a1, int a2, int id, int a4, int a5)
{
    if (id != -1) {
        Rec08B4C4FC *r = (Rec08B4C4FC *)func_088C0FE8(id);
        Ptrs08B4C4FC *q = &r->q;
        int v = func_08B4B040(*q->p20, *q->p28, *q->p10, *q->p18, a4, a5);
        int w = func_0887AD78(a2, 0, r->s8, 0, 0, 0);
        func_08B4C264(a0, a1, v, w);
    }
}
