/* Starts receiving into `data` when no receive is running; returns whether it started. */
struct Transfer {
    char pad_00[0x1C];
    void *data;
    int size;
    int done;
    int mode;
};

extern "C" {
bool func_08905958(Transfer *t, void *data, int size)
{
    if (data == 0) {
        return false;
    }
    if (t->data == 0) {
        t->done = 0;
        t->data = data;
        t->size = size;
        t->mode = 2;
        return true;
    }
    return false;
}
}
