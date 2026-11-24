/* Appends a 6-word record to the slot's list (max 0x2D) when in state 1; -1..-4 on the failure cases. */
typedef struct R6 { unsigned a; unsigned kind; unsigned c; unsigned d; unsigned e; unsigned f; } R6;
typedef struct Slot { unsigned char active; char pad[3]; int state; char pad_08[0x278]; unsigned count; R6 recs[0x2D]; } Slot;
int func_08B1A7F4(Slot *self, R6 *src)
{
    if (self->active == 0) {
        return -1;
    }
    if (src == 0 || src->kind > 4) {
        return -2;
    }
    if (self->state != 1) {
        return -3;
    }
    if (self->count >= 0x2D) {
        return -4;
    }
    self->recs[self->count] = *src;
    self->count = self->count + 1;
    return 0;
}
