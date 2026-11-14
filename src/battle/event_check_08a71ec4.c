typedef struct EventOwner71EC4 {
    unsigned char active;
} EventOwner71EC4;

extern int func_08A6BDCC(EventOwner71EC4 *self);

int func_08A71EC4(EventOwner71EC4 *self, void *a, void *b, void *c)
{
    if (!a || !b || !c) {
        return -1;
    }
    if (func_08A6BDCC(self)) {
        return -2;
    }
    return 0;
}
