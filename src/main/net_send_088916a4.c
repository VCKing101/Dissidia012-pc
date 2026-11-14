/* Sends data on the connection: -1 inactive, -2 without data, -3 on failure. */
typedef struct Conn088916A4 {
    unsigned char active;
    char pad_01[0x17];
    int channel;
} Conn088916A4;

int func_08891C18(Conn088916A4 *c, void *data, int channel);

int func_088916A4(Conn088916A4 *c, void *data)
{
    if (!c->active) {
        return -1;
    }
    if (data == 0) {
        return -2;
    }
    if (func_08891C18(c, data, c->channel) != 0) {
        return -3;
    }
    return 0;
}
