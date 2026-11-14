typedef struct EventOwner70630 {
    unsigned char active;
} EventOwner70630;

extern int func_08A6D134(EventOwner70630 *self);
extern int func_08A6D24C(EventOwner70630 *self);

int func_08A70630(EventOwner70630 *self, void *a, void *b, void *c)
{
    if (!a || !b || !c) {
        return -1;
    }
    if (func_08A6D134(self)) {
        return -2;
    }
    return 0;
}

int func_08A70680(EventOwner70630 *self, void *a, void *b, void *c)
{
    if (!a || !b || !c) {
        return -1;
    }
    if (func_08A6D24C(self)) {
        return -2;
    }
    return 0;
}
