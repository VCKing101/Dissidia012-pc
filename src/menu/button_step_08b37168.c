/* Reads the page button state: 1 refreshes the page, 3 confirms it, clears its flag and moves to state 6. */
extern int func_089315D4(void *w);
extern int func_089315DC(void *w);
extern void func_08B3BB38(void *self, int a, int b);
extern void func_0893156C(void *w);
extern void func_08B3A25C(void *self, int a, int b);
extern void func_08B4E794(void *w);
extern void func_0892FFB4(void *w);

typedef struct W37168 { char pad[0x14]; void *w14; char pad18[0xC]; void *w24; char pad28[0x14]; void *w3C; } W37168;
typedef struct S37168 { char pad[0x44]; unsigned flags; char pad48[0x7C]; int *state; W37168 *w; } S37168;

void func_08B37168(S37168 *self)
{
    int b = func_089315D4(self->w->w14);
    int v = func_089315DC(self->w->w14);
    switch (b) {
    case 0:
        break;
    case 1:
        func_08B3BB38(self, v, 0);
        break;
    case 2:
        break;
    case 3:
        func_0893156C(self->w->w14);
        func_08B3A25C(self, v, 1);
        self->flags = self->flags & 0xFFFFFEFF;
        func_08B4E794(self->w->w24);
        func_0892FFB4(self->w->w3C);
        *self->state = 6;
        break;
    }
}
