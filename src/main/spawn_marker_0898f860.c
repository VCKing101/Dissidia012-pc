/* If func_0898F654 allows it, initialises marker a1 from the position at +0xAA and starts its effect with the short at +0x9E. */
typedef struct S { char p[0x9E]; short eff; char q[0xA]; short pos[3]; } S;
extern int func_0898F654(S *self);
extern void func_0898EE14(void *m, int a, void *pos, int b, int c);
extern void func_0898EE94(void *m, int a, int eff, int one, float f, float g);
void func_0898F860(S *self, void *m, int a, int b) {
    if (func_0898F654(self) != 0) {
        func_0898EE14(m, b, (char *)self + 0xAA, 0, 1);
        func_0898EE94(m, a, self->eff, 1, 0.0f, 0.0f);
    }
}
