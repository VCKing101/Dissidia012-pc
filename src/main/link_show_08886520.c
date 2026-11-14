typedef struct Target08886520 {
    unsigned char unknown_00[0xE];
    short shown;
} Target08886520;

typedef struct Link08886520 {
    int handle;
    Target08886520 *target;
    unsigned int flags;
} Link08886520;

void func_088D9F5C(Target08886520 *target, int on, int now);

void func_08886520(Link08886520 *self, unsigned char on) {
    if (self->target != 0) {
        self->flags |= 1;
        if (on) {
            func_088D9F5C(self->target, 1, 1);
        } else {
            func_088D9F5C(self->target, 0, 1);
            self->target->shown = 1;
        }
    }
}
