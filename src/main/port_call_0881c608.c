/* Forwards a four-argument request to the device's port unless the device is
   idle (state 1): -1 when inactive, -2 when the port call fails. */
typedef struct Device0881C608 {
    unsigned char active;
    char pad_01[7];
    unsigned char state;
    char pad_09[0xB];
    unsigned char port[4];
} Device0881C608;

int func_08816BA8(void *port, int a, int b, int c, int d);

int func_0881C608(Device0881C608 *self, int a, int b, int c, int d)
{
    if (self->active == 0) {
        return -1;
    }
    if (self->state == 1) {
        return 0;
    }
    if (func_08816BA8(self->port, a, b, c, d) != 0) {
        return -2;
    }
    return 0;
}
