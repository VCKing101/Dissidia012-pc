/* If not already started: clear the timer, run the start call, record src's id and mark started. */
typedef struct Src { char p[0x8E]; unsigned short id; } Src;
typedef struct S { char p[0x50]; int id; char q[4]; signed char started; unsigned char flag; char r[0xA]; float t; int t2; } S;
extern void func_0898E564(S *s, Src *src, int f);
void func_0898E120(S *self, Src *src, unsigned char f) {
    if (self->started) return;
    self->t = 0.0f;
    self->t2 = 0;
    func_0898E564(self, src, f);
    self->id = src->id;
    self->flag = (self->id == 0);
    self->started = 1;
}
