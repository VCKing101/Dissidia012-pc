/* With a zero argument, sends two messages to the sub-view and shows or hides its child by the mode byte. */
typedef struct Vw { char pad0[0x34]; int child; } Vw;
typedef struct Own { char pad0[0x140]; char *p; } Own;
extern void func_088D428C(void *, int, const void *, int);
extern void func_088D9CD8(int);
extern void func_088D9CFC(int);
extern const char D_08B006BE[], D_08B00726[], D_08B007EE[], D_08B0085E[];
typedef struct S2 { char pad0[4]; Vw *a; int b; char pad1[0x20 - 0xC]; unsigned char mode; } S2;
void func_08AF4F6C(Own *self, int arg) {
    S2 *s;
    Vw *a;
    int b;
    unsigned char m;
    if (arg == 0) {
        s = (S2 *)(self->p + 0x2C40);
        m = s->mode;
        a = s->a;
        b = s->b;
        if (m == 0) {
            func_088D428C(a, b, D_08B006BE, 1);
            func_088D428C(s->a, s->b, D_08B00726, 0);
            func_088D9CD8(s->a->child);
        } else {
            func_088D428C(a, b, D_08B007EE, 1);
            func_088D428C(s->a, s->b, D_08B0085E, 0);
            func_088D9CFC(s->a->child);
        }
    }
}
