// Seeks the stream to the saved position, ends it when the end marker has been reached, and clears the 0x10 flag.
struct S { char pad[4]; int h; char pad2[8]; int pos; int skip; int a; int b; char pad3[0xC]; unsigned flags; };
extern void func_089C5CD0(int, int, int, int);
extern void func_089C5CF0(int, int);
void func_08992BB0(struct S *s) {
    func_089C5CD0(s->h, s->pos, 0, 0);
    if (s->a > -1 && s->b > -1 && s->pos <= s->b) {
        func_089C5CF0(s->h, -1);
    }
    s->flags = s->flags & 0xFFFFFFEF;
}
