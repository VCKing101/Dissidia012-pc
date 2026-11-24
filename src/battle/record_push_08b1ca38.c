/* Appends a 6-word record to the slot's list (max 0x80) when in state 0; -1..-4 on the failure cases. */
typedef struct R6 { unsigned a; unsigned kind; unsigned c; unsigned d; unsigned e; unsigned f; } R6;
typedef struct Slot { unsigned char active; char pad[3]; int pad4; int state; char pad_0c[0x2018]; unsigned count; R6 recs[0x80]; } Slot;
int func_08B1CA38(Slot *self, R6 *src)
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
    if (self->count >= 0x80) {
        return -4;
    }
    self->recs[self->count] = *src;
    self->count = self->count + 1;
    return 0;
}
