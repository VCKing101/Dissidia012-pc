// Updates each of the five active slots, then every other effect index 1..0x20 except 0x17.
struct Slot { int active; char d[0xD4]; };
struct S { struct Slot slot[5]; };
extern void func_088F37BC(struct S *s, int flag, int i);
extern void func_088F3B80(struct S *s, int i);
void func_088F3A7C(struct S *s, unsigned char flag)
{
    int i;
    int skip;
    struct Slot *p;
    for (i = 0, p = s->slot; i < 5; i++, p++) {
        if (p->active != 0)
            func_088F37BC(s, flag, i);
    }
    for (i = 1, skip = 0x17; i < 0x21; i++) {
        if (i != skip)
            func_088F3B80(s, i);
    }
}
