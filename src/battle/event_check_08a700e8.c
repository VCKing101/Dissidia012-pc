typedef struct EventOwner700E8 {
    unsigned char active;
} EventOwner700E8;

extern int func_08A75D38(EventOwner700E8 *self, int a, int b, int c, int d);

int func_08A700E8(EventOwner700E8 *self, void *a, void *b, void *c)
{
    int result = -1;
    if (!a || !b || !c) {
        return result;
    }
    if (func_08A75D38(self, 0, 0, -1, 0)) {
        return -2;
    }
    return 0;
}
