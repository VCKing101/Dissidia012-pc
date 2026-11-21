/* Releases the unit's handles when it is in state 2, then moves it to state 5; -1/-2/-3 on bad state. */
typedef struct Own38 Own38;
typedef struct Obj38 { unsigned char active; char pad[0xF]; int state; void *h14; void *h18; int pad1c; void *h20; char pad2[0xB4]; Own38 *owner; } Obj38;
struct Own38 { char pad[0x38]; void *x; };
extern void func_08AB6A80(void *h);
extern void func_08B333A4(void *a, void *b);
int func_08AC1610(Obj38 *self, Own38 *owner)
{
    if (self->active == 0) {
        return -1;
    } else if (owner == 0) {
        return -2;
    } else if (self->state == 2) {
        func_08AB6A80(self->h14);
        self->h14 = 0;
        if (self->h18 != 0) {
            func_08AB6A80(self->h18);
            self->h18 = 0;
        }
        func_08B333A4(owner->x, self->h20);
        self->h20 = 0;
        self->state = 5;
        self->owner = owner;
        return 0;
    } else {
        return -3;
    }
}
