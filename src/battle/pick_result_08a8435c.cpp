/* Tries three checks in turn and stores which one fired (1, 0, 2) in *out; returns 1 on a hit. */
extern "C" {
int func_08A8477C(void *u, int a, int b, int c, int d);
int func_08A857A8(void *u, int a, int b, int c);
int func_08A849C8(void *u, int a, int b);
bool func_08A8435C(void *u, int a, int b, int c, int d, int e, int *out)
{
    if (u == 0 || out == 0) {
        return false;
    }
    if (func_08A8477C(u, a, b, d, e) == 1) {
        *out = 1;
        return true;
    } else {
        if (func_08A857A8(u, a, b, c) == 1) {
            *out = 0;
            return true;
        } else {
            if (func_08A849C8(u, a, b) == 1) {
                *out = 2;
                return true;
            } else {
                return false;
            }
        }
    }
}
}
