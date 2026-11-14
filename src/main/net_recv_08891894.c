/* Receives into buf: -1 inactive, -2 without a buffer or with a negative size, -3 on failure. */
typedef struct Conn08891894 {
    unsigned char active;
    char pad_01[3];
    int peer;
} Conn08891894;

int func_08891CA4(Conn08891894 *c, void *buf, int peer, int size);

int func_08891894(Conn08891894 *c, void *buf, int size)
{
    if (!c->active) {
        return -1;
    }
    if (buf == 0 || size < 0) {
        return -2;
    }
    if (func_08891CA4(c, buf, c->peer, size) != 0) {
        return -3;
    }
    return 0;
}
