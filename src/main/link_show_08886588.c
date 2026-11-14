typedef struct Target08886588 {
    unsigned char unknown_00[0xE];
    short shown;
} Target08886588;

typedef struct Link08886588 {
    int handle;
    Target08886588 *target;
    unsigned int flags;
} Link08886588;

void func_088D9F5C(Target08886588 *target, int on, int now);

void func_08886588(Link08886588 *self, unsigned char on) {
    if (self->target != 0) {
        self->flags |= 1;
        if (on) {
            func_088D9F5C(self->target, -1, 1);
        } else {
            func_088D9F5C(self->target, 2, 1);
            self->target->shown = -1;
        }
    }
}
