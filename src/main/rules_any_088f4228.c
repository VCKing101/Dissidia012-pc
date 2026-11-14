extern int func_088F414C(void *ctx, int id, int flags);

int func_088F4228(void *ctx)
{
    int found = 0;
    int i;
    for (i = 1; i < 33; i++) {
        if (i != 23) {
            if (func_088F414C(ctx, i, 0) != 0) {
                found = 1;
                break;
            }
        }
    }
    return found;
}
