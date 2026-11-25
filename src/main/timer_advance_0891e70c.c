/* advance state 2 to state 3: start the timer pair. */
extern int func_088D4C5C(void);
extern void func_08873DEC(unsigned char a, int b);
extern void func_08873D34(int a, void *b, void *c, int d, int e, float f);
typedef struct Obj { char pad[0x18]; int state; } Obj;
void func_0891E70C(Obj *self)
{
    if (self->state == 2) {
        int t = func_088D4C5C();
        int next = 3;
        func_08873DEC(3, 2);
        func_08873D34(3, (void *)0x0891E888, self, 1, 2, (float)t);
        self->state = next;
    }
}
