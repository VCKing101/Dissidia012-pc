/* Frees the handles in slots 26..32 of the list at +0x49C, then rebuilds six rows starting at the slot 3 before the current one (wrapping at 5): each gets func_0887E534 of its handle and an update via func_088D9F5C, finally storing the current row's result. */
typedef struct P8ACE8 {
    char p0[0x20]; int obj[6];
    char p1[0x49C - 0x38]; int h[40];
    char p3[0x740 - 0x49C - 160]; int sel;
} P8ACE8;
typedef struct S8ACE8 { char pad[0xA18]; P8ACE8 *p; } S8ACE8;
typedef struct E8ACE8 { char p[6]; short on; char q[72 - 8]; } E8ACE8;
extern char *D_089D712C;
extern void func_0887BA5C(int h);
extern int func_0887E534(int h);
extern void func_088D9F5C(int obj, int mode, int a);
void func_08B2ACE8(S8ACE8 *self)
{
    int off;
    int j;

    int s;
    E8ACE8 *t;
    j = 0x1A;
    t = (E8ACE8 *)self->p;

    for (off = 0x68; j < 0x21; j++, off += 4) {
        if (*(int *)((char *)self->p + off + 0x49C) != 0) {
            func_0887BA5C(*(int *)((char *)self->p + off + 0x49C));
            *(int *)((char *)self->p + off + 0x49C) = 0;
        }
    }
    s = self->p->sel - 3;
    if (s < 0) {
        s += 5;
    }
    t = (E8ACE8 *)(D_089D712C + 0x3E758);
    for (j = 0, off = 0; j < 6; j++, off += 4) {
        int mode;
        *(int *)((char *)self->p + off + 0x508) = func_0887E534(self->p->h[s]);
        mode = 2;
        if (t[s].on != 0) {
            mode = 3;
        }
        func_088D9F5C(*(int *)((char *)self->p + off + 0x20), mode, 1);
        s++;
        if (s >= 5) {
            s = 0;
        }
    }
    self->p->h[0x1A] = func_0887E534(self->p->h[self->p->sel]);
}
