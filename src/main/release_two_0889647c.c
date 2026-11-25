/* Stops the two handles at +0x3C and +0x40 (volume 0), runs two release calls with a1/a2, then clears the handles. */
typedef struct S { char p[0x3C]; int h1; int h2; } S;
extern void func_088A2664(int h, float f);
extern void func_088A24D4(int a, int b, int c, int d);
void func_0889647C(S *self, int a, int b) {
    float zero = 0.0f;
    func_088A2664(self->h1, zero);
    func_088A2664(self->h2, zero);
    func_088A24D4(0, a, 1, 0);
    func_088A24D4(0, b, 1, 0);
    self->h1 = 0;
    self->h2 = 0;
}
