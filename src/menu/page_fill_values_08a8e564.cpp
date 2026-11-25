/* Menu page fill: copies the five source values into the page's value slots, or clears them and stores the default into slot 0. */
struct Pair { int v; int x; };
struct Dst { char p[0x14]; int v[5]; };
struct Src { char p[0x104]; Pair s[5]; };
struct Owner { char p[0x2B4]; int sel; int pad; int mode[1]; };
struct Self { char p0[8]; Owner *own; char p1[0x14]; Dst *dst; Src *src; };
extern "C" {
int func_08A8E01C(Self *);
int func_08A8EE9C(Self *);
void func_08A8E564(Self *self)
{
    int i;
    if (*(int *)((char *)self->own + self->own->sel * 4 + 0x308) == 2) {
        for (i = 0; i < 5; i++) {
            self->dst->v[i] = self->src->s[i].v;
        }
    } else if (func_08A8E01C(self) == 1) {
        for (i = 0; i < 5; i++) {
            self->dst->v[i] = self->src->s[i].v;
        }
    } else {
        for (i = 0; i < 5; i++) {
            self->dst->v[i] = 0;
        }
        self->dst->v[0] = func_08A8EE9C(self);
    }
}
}
