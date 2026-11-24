/* Opens a handle: func_089C5A28 into a local, then func_089C59B8(local, id & 0xFFFF, size, 0); on success fills out {handle, id, size} and returns 1, else 0. */
extern int func_089C5A28(void *tmp);
extern int func_089C59B8(void *tmp, unsigned a, int b, int c);
int func_089055C0(int *out, unsigned id, int size)
{
    int tmp[2];
    int h;
    if (func_089C5A28(tmp) < 0) {
        return 0;
    } else {
        h = func_089C59B8(tmp, id & 0xFFFF, size, 0);
        if (h < 0) {
            return 0;
        } else {
            out[0] = h;
            out[1] = id;
            out[2] = size;
            return 1;
        }
    }
}
