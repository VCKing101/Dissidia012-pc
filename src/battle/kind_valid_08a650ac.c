typedef struct KindSet {
    unsigned char active;
} KindSet;

/* True for kinds 0-5 while the set is active. */
int func_08A650AC(KindSet *self, int kind)
{
    if (self->active == 0) {
        return 0;
    }
    switch (kind) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        return 1;
    default:
        return 0;
    }
}
