/* Forwards a request to the port's channel: -1 when closed, -2 on failure. */
typedef struct Port08816B3C {
    unsigned char active;
    unsigned char unknown_01[3];
    unsigned char channel[4];
} Port08816B3C;

int func_0881B278(void *channel, int a, int b, int c, int d);

int func_08816B3C(Port08816B3C *self, int a, int b, int c, int d)
{
    if (self->active == 0) {
        return -1;
    }
    if (func_0881B278(self->channel, a, b, c, d) != 0) {
        return -2;
    }
    return 0;
}
