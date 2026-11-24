/* Stores a 6-word record into an idle slot; -1 inactive, -2 bad source, -3 slot already busy. */
typedef struct R6 { unsigned a; unsigned kind; unsigned c; unsigned d; unsigned e; unsigned f; } R6;
typedef struct Slot { unsigned char active; char pad[3]; int state; R6 rec; } Slot;
int func_08B1B0D4(Slot *self, R6 *src)
{
    if (self->active == 0) {
        return -1;
    }
    if (src == 0 || src->kind > 0x20) {
        return -2;
    }
    if (self->state != 0) {
        return -3;
    }
    self->rec = *src;
    self->state = 1;
    return 0;
}
