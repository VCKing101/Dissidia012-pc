typedef struct EventOwner6E9E8 {
    unsigned char active;
} EventOwner6E9E8;

extern int func_08A6B0A0(EventOwner6E9E8 *self);

int func_08A6E9E8(EventOwner6E9E8 *self, void *a, void *b, void *c)
{
    if (!a || !b || !c) {
        return -1;
    }
    if (func_08A6B0A0(self)) {
        return -2;
    }
    return 0;
}
