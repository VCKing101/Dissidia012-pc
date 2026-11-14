/* Starts a transfer of `data` when none is running; returns whether it started. */
struct Transfer {
    char pad_00[0xC];
    void *data;
    int size;
    int done;
    unsigned char failed;
    char pad_19[0x28 - 0x19];
    int active;
};

extern "C" {
bool func_08905910(Transfer *t, void *data, int size)
{
    if (data == 0) {
        return false;
    }
    if (t->data == 0) {
        t->done = 0;
        t->failed = 0;
        t->data = data;
        t->size = size;
        t->active = 1;
        return true;
    }
    return false;
}
}
