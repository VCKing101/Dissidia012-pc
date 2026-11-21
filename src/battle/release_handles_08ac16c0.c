/* Releases the two handles (if set) and clears the state fields; -1 when inactive. */
typedef struct Hd38 { unsigned char active; char pad[0xF]; int state; void *h14; void *h18; float a; int b; char pad2[0xB4]; int c; } Hd38;
extern void func_08AB6A80(void *h);
int func_08AC16C0(Hd38 *self)
{
    if (self->active == 0) {
        return -1;
    }
    if (self->h14 != 0) {
        func_08AB6A80(self->h14);
        self->h14 = 0;
    }
    if (self->h18 != 0) {
        func_08AB6A80(self->h18);
        self->h18 = 0;
    }
    self->state = 0;
    self->a = 0.0f;
    self->b = 0;
    self->c = 0;
    return 0;
}
