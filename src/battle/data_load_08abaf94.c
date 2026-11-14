typedef struct LoadOwner LoadOwner;

extern void func_08ABAE40(LoadOwner *self);
extern int func_08AB9B60(LoadOwner *self, void *data, int size);

/* Reset, then load size bytes of data; -1 for no data, -2 when loading fails. */
int func_08ABAF94(LoadOwner *self, void *data, int size)
{
    func_08ABAE40(self);
    if (data == 0 || size <= 0) {
        return -1;
    }
    if (func_08AB9B60(self, data, size) != 0) {
        return -2;
    }
    return 0;
}
