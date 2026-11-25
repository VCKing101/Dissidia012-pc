/* Menu per-frame timer: counts the countdown down, running the step handlers at 1 and 0; with no countdown draws the two overlays. */
struct W { char p0[0x1408]; int ov; char p1[8]; int timer; };
struct Self { char p[0xDB30]; W *w; };
extern "C" {
extern char D_08AD98AC[], D_08AD98D4[], D_08AD9914[];
void func_088D3F00(void *, void *, void *);
void func_088D44EC(void *, void *, void *);
void func_08AD1924(Self *, int);
void func_08ACB280(Self *self)
{
    func_088D3F00(self->w, (char *)self->w + 0x400, D_08AD98AC);
    if (self->w->timer != 0) {
        self->w->timer = self->w->timer - 1;
        if (self->w->timer == 1) {
            func_08AD1924(self, 1);
        } else if (self->w->timer <= 0) {
            func_08AD1924(self, 0);
            self->w->timer = 0;
        }
    } else if (self->w->ov != -1) {
        func_088D44EC((char *)self->w + 0x400, (char *)self->w + 0x880, D_08AD98D4);
        func_088D44EC((char *)self->w + 0x400, (char *)self->w + 0x880, D_08AD9914);
    }
}
}
