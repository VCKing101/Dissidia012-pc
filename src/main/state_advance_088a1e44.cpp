struct Hnd { virtual void v0(); virtual void v1(); virtual void v2(); };
struct Ob { char pad[0x30]; int state; unsigned flags; char pad2[0x50 - 0x38]; Hnd *h; int w54; };
extern "C" void func_088A1F2C(Ob *, float, float);
extern "C" void func_088A1E44(float f, Ob *self) {
    if (f == 0.0f) {
        switch (self->state) {
        case 0: self->state = 2; break;
        case 1: self->h->v2(); break;
        case 2: break;
        default: break;
        }
        self->w54 = 0;
    } else {
        switch (self->state) {
        case 0: self->state = 2; break;
        case 1: func_088A1F2C(self, 0.0f, f); self->flags |= 0x100; break;
        case 2: break;
        }
    }
}
